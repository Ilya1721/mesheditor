#include "Renderer.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"
#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"
#include "MeshCore/Object3D.h"
#include "MeshCore/Mesh.h"
#include "MeshCore/RootRenderDataStorage.h"

#include "RenderLogger.h"
#include "GladTypedefs.h"
#include "RenderPrimitive.h"
#include "GlobalExtraPrimitives.h"
#include "Scene.h"
#include "GlobalRenderState.h"

using namespace MeshCore;

namespace
{
	using namespace RenderSystem;

	constexpr Material MAIN_MATERIAL = GOLD_MATERIAL;
	constexpr Material HIGHLIGHT_MATERIAL = RUBY_MATERIAL;
	constexpr Material WIREFRAME_MATERIAL = BLACK_MATERIAL;

	void checkShaderOrProgramStatus(GetShaderIV getShaderIVFunc, int shaderOrProgram, int statusToCheck,
									SHADER_TYPE shaderType, const std::string& failedMessage)
	{
		int isStatusSuccessful = false;
		getShaderIVFunc(shaderOrProgram, statusToCheck, &isStatusSuccessful);
		if (!isStatusSuccessful)
		{
			auto shaderLog = getShaderInfoLog(shaderOrProgram, shaderType);
			std::cerr << shaderLog << std::endl;
			throw std::exception(failedMessage.c_str());
		}
	}

	int loadShader(const std::string& shaderPath, int shaderType)
	{
		auto shaderContent = Utility::readFile(shaderPath);
		std::vector<const char*> shaderContentVec{ shaderContent.c_str() };
		std::vector<int> shaderContentLengthVec{ static_cast<int>(shaderContent.size()) };

		auto shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, shaderContentVec.data(), shaderContentLengthVec.data());
		glCompileShader(shader);
		checkShaderOrProgramStatus(glGetShaderiv, shader, GL_COMPILE_STATUS, SHADER_TYPE::SHADER, "Shader was not compiled!");

		return shader;
	}
}

namespace RenderSystem
{
	Renderer::Renderer() :
		mVertexShader(),
		mFragmentShader(),
		mShaderProgram(),
		mLighting(),
		mRenderBuffer(),
		mExtraRenderBuffer(),
		mShaderTransformationSystem()
	{
		init();
	}

	Renderer::~Renderer()
	{
		glUseProgram(0);
		glDeleteShader(mVertexShader);
		glDeleteShader(mFragmentShader);
		glDeleteProgram(mShaderProgram);
	}

	void Renderer::init()
	{
		initShaders();
		mShaderTransformationSystem.init(mShaderProgram);
		mShaderTransformationSystem.setModel(glm::value_ptr(Scene::getRootObject().getTransform()));
		mLighting.init(mShaderProgram);
		registerCallbacks();
		mRenderBuffer.bind();
	}

	void Renderer::registerCallbacks()
	{
		RootRenderDataStorage::addOnRenderDataUpdatedCallback([this]() {
			mRenderBuffer.loadRenderData(RootRenderDataStorage::getRenderData());
		});
		RootRenderDataStorage::addOnExtraRenderDataUpdatedCallback([this]() {
			mExtraRenderBuffer.bind();
			mExtraRenderBuffer.loadRenderData(RootRenderDataStorage::getExtraRenderData());
			mRenderBuffer.bind();
		});
	}

	void Renderer::initShaders()
	{
		mVertexShader = loadShader(R"(../RenderSystem/Shaders/VertexShader.vert)", GL_VERTEX_SHADER);
		mFragmentShader = loadShader(R"(../RenderSystem/Shaders/FragmentShader.frag)", GL_FRAGMENT_SHADER);
		initShaderProgram();
	}

	void Renderer::initShaderProgram()
	{
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, mVertexShader);
		glAttachShader(mShaderProgram, mFragmentShader);
		glLinkProgram(mShaderProgram);
		checkShaderOrProgramStatus(glGetProgramiv, mShaderProgram, GL_LINK_STATUS, SHADER_TYPE::SHADER_PROGRAM, "Shader program was not linked");
		glUseProgram(mShaderProgram);
	}

	void Renderer::renderHighlightedFaces()
	{
		const auto& highlightedFacesData = GlobalRenderState::getHighlightedFacesData();
		const auto& objectVertexCountMap = Object3D::getObjectVertexCountMap();
		renderOverlayPrimitives(!highlightedFacesData.facesIndices.empty(), HIGHLIGHT_MATERIAL,
			[&highlightedFacesData, &objectVertexCountMap, this]() {
			for (const auto& faceIdx : highlightedFacesData.facesIndices)
			{
				mShaderTransformationSystem.setModel(glm::value_ptr(highlightedFacesData.parentObject->getTransform()));
				glDrawArrays(GL_TRIANGLES, faceIdx * 3 + objectVertexCountMap.at(highlightedFacesData.parentObject), 3);
			}
		});
	}

	void Renderer::renderWireframe()
	{
		renderOverlayPrimitives(GlobalRenderState::getRenderWireframe(), WIREFRAME_MATERIAL, [this]() {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, RootRenderDataStorage::getRenderData().getVertexCount());
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		});
	}

	void Renderer::renderWholeObjectHighlighted()
	{
		const auto& objectVertexCountMap = Object3D::getObjectVertexCountMap();
		const auto& highlightedObjectIt = objectVertexCountMap.find(GlobalRenderState::getHighlightedObject());
		renderOverlayPrimitives(highlightedObjectIt != objectVertexCountMap.end(), HIGHLIGHT_MATERIAL, [this, highlightedObjectIt]() {
			auto& [object, vertexCount] = *highlightedObjectIt;
			mShaderTransformationSystem.setModel(glm::value_ptr(object->getTransform()));
			glDrawArrays(GL_TRIANGLES, vertexCount, object->getMesh().getVertices().size());
		});
	}

	void Renderer::renderScene()
	{
		for (auto& [object, vertexCount] : Object3D::getObjectVertexCountMap())
		{
			mShaderTransformationSystem.setModel(glm::value_ptr(object->getTransform()));
			glDrawArrays(GL_TRIANGLES, vertexCount, object->getMesh().getVertices().size());
		}
	}

	void Renderer::renderOverlayPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc)
	{
		if (!renderCondition)
		{
			return;
		}

		mLighting.setMaterial(material);
		glDepthFunc(GL_LEQUAL);
		renderFunc();
		glDepthFunc(GL_LESS);
		mLighting.setMaterial(MAIN_MATERIAL);
	}

	void Renderer::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderScene();
		renderHighlightedFaces();
		renderWireframe();
		renderWholeObjectHighlighted();
		renderExtra();
	}

	void Renderer::renderExtra()
	{
		mExtraRenderBuffer.bind();
		mShaderTransformationSystem.setModel(glm::value_ptr(glm::mat4(1.0f)));

		int startIndex = 0;
		for (const auto& extraPrimitive : GlobalExtraPrimitives::getExtraPrimitives())
		{
			auto vertexCount = extraPrimitive.renderData.getVertexCount();
			mLighting.setMaterial(extraPrimitive.material);
			glDrawArrays(extraPrimitive.renderMode, startIndex, vertexCount);
			startIndex += vertexCount;
		}

		mLighting.setMaterial(MAIN_MATERIAL);
		mShaderTransformationSystem.setModel(glm::value_ptr(Scene::getRootObject().getTransform()));
		mRenderBuffer.bind();
	}

	ShaderTransformationSystem& Renderer::getShaderTransformationSystem()
	{
		return mShaderTransformationSystem;
	}

	Lighting& Renderer::getLighting()
	{
		return mLighting;
	}

	RenderBuffer& Renderer::getRenderBuffer()
	{
		return mRenderBuffer;
	}
}
