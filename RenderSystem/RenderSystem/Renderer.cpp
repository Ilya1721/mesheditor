#include "Renderer.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "Utility/FileHelper.h"
#include "Utility/StringHelper.h"
#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"
#include "MeshCore/Mesh.h"

#include "RenderLogger.h"
#include "GladTypedefs.h"
#include "GlobalRenderState.h"
#include "Object3D.h"

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
		mModelRenderBuffer(),
		mDecorationsRenderBuffer(),
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

	void Renderer::setModel(const float* model) const
	{
		mShaderTransformationSystem.setModel(model);
	}

	void Renderer::setView(const float* view) const
	{
		mShaderTransformationSystem.setView(view);
	}

	void Renderer::setProjection(const float* projection) const
	{
		mShaderTransformationSystem.setProjection(projection);
	}

	void Renderer::setLightPos(const float* pos) const
	{
		mLighting.setLightPos(pos);
	}

	void Renderer::setCameraPos(const float* pos) const
	{
		mLighting.setCameraPos(pos);
	}

	void Renderer::init()
	{
		initShaders();
		mShaderTransformationSystem.init(mShaderProgram);
		mLighting.init(mShaderProgram);
		mModelRenderBuffer.bind();
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

	void Renderer::renderHighlightedFaces(const std::unordered_map<const Object3D*, int>& objectVertexOffsetMap)
	{
		const auto& highlightedFacesData = GlobalRenderState::getHighlightedFacesData();
		renderOverlayPrimitives(!highlightedFacesData.facesIndices.empty(), HIGHLIGHT_MATERIAL,
		[&highlightedFacesData, &objectVertexOffsetMap, this]() {
			for (const auto& faceIdx : highlightedFacesData.facesIndices)
			{
				setModel(glm::value_ptr(highlightedFacesData.parentObject->getTransform()));
				glDrawArrays(GL_TRIANGLES, faceIdx * 3 + objectVertexOffsetMap.at(highlightedFacesData.parentObject), 3);
			}
		});
	}

	void Renderer::renderWireframe(int sceneVertexCount)
	{
		renderOverlayPrimitives(GlobalRenderState::getRenderWireframe(), WIREFRAME_MATERIAL, [&sceneVertexCount]() {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, sceneVertexCount);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		});
	}

	void Renderer::renderWholeObjectHighlighted(const std::unordered_map<const Object3D*, int>& objectVertexOffsetMap)
	{
		const auto& highlightedObjectIt = objectVertexOffsetMap.find(GlobalRenderState::getHighlightedObject());
		renderOverlayPrimitives(highlightedObjectIt != objectVertexOffsetMap.end(), HIGHLIGHT_MATERIAL, [this, highlightedObjectIt]() {
			auto& [object, vertexCount] = *highlightedObjectIt;
			setModel(glm::value_ptr(object->getTransform()));
			glDrawArrays(GL_TRIANGLES, vertexCount, object->getVertexCount());
		});
	}

	void Renderer::renderScene(const std::unordered_map<const Object3D*, int>& objectVertexOffsetMap)
	{
		for (auto& [object, vertexOffset] : objectVertexOffsetMap)
		{
			setModel(glm::value_ptr(object->getTransform()));
			glDrawArrays(GL_TRIANGLES, vertexOffset, object->getVertexCount());
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

	void Renderer::cleanScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::loadDecorationsRenderData(const RenderData& renderData)
	{
		mDecorationsRenderBuffer.bind();
		mDecorationsRenderBuffer.loadRenderData(renderData);
		mModelRenderBuffer.bind();
	}

	void Renderer::loadModelRenderData(const RenderData& renderData)
	{
		mModelRenderBuffer.loadRenderData(renderData);
	}

	void Renderer::renderSceneDecorations(const std::vector<SceneDecoration>& sceneDecorations, const glm::mat4& rootObjectTransform)
	{
		mDecorationsRenderBuffer.bind();
		setModel(glm::value_ptr(glm::mat4(1.0f)));

		int startIndex = 0;
		for (const auto& sceneDecoration : sceneDecorations)
		{
			auto vertexCount = sceneDecoration.renderData.getVertexCount();
			mLighting.setMaterial(sceneDecoration.material);
			glDrawArrays(sceneDecoration.renderMode, startIndex, vertexCount);
			startIndex += vertexCount;
		}

		mLighting.setMaterial(MAIN_MATERIAL);
		setModel(glm::value_ptr(rootObjectTransform));
		mModelRenderBuffer.bind();
	}
}
