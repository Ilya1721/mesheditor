#include "Renderer.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"
#include "GeometryCore/Transforms.h"
#include "MeshCore/Object3D.h"
#include "MeshCore/Mesh.h"
#include "MeshCore/RootRenderDataStorage.h"

#include "RenderPrimitive.h"
#include "GlobalExtraPrimitives.h"
#include "GlobalRenderState.h"
#include "Texture.h"
#include "Camera.h"
#include "Window.h"

using namespace MeshCore;

namespace
{
	using namespace RenderSystem;

	constexpr Material MAIN_MATERIAL = GOLD_MATERIAL;
	constexpr Material HIGHLIGHT_MATERIAL = RUBY_MATERIAL;
	constexpr Material WIREFRAME_MATERIAL = BLACK_MATERIAL;

	const std::string MAIN_VERTEX_SHADER_PATH = R"(../Shaders/VertexShader.frag)";
	const std::string MAIN_FRAGMENT_SHADER_PATH = R"(../Shaders/FragmentShader.frag)";
	const std::string DEPTH_VERTEX_SHADER_PATH = R"(../Shaders/DepthMapVertexShader.vert)";
	const std::string DEPTH_FRAGMENT_SHADER_PATH = R"(../Shaders/DepthMapFragmentShader.frag)";

	GlobalRenderState* gGlobalRenderState = nullptr;
	Camera* gCamera = nullptr;
	RootRenderDataStorage* gRootRenderDataStorage = nullptr;
	GlobalExtraPrimitives* gGlobalExtraPrimitives = nullptr;
}

namespace RenderSystem
{
	Renderer& Renderer::getInstance()
	{
		static Renderer sInstance;
		return sInstance;
	}

	Renderer::Renderer() :
		mExtraFBO(),
		mRenderBuffer(),
		mExtraRenderBuffer(),
		mSceneShaderProgram(),
		mDepthShaderProgram()
	{
		gGlobalRenderState = &gGlobalRenderState->getInstance();
		gCamera = &Camera::getInstance();
		gRootRenderDataStorage = &RootRenderDataStorage::getInstance();
		gGlobalExtraPrimitives = &GlobalExtraPrimitives::getInstance();
		gGlobalRenderState->addRootObjectInitializedCallback([this]() {
			init();
		});
	}

	Renderer::~Renderer()
	{
		glDeleteFramebuffers(1, &mExtraFBO);
	}

	void Renderer::addInitializedCallback(const std::function<void()>& callback)
	{
		mRendererInitCM.addCallback(callback);
	}

	void Renderer::init()
	{
		mSceneShaderProgram.init(MAIN_VERTEX_SHADER_PATH, MAIN_FRAGMENT_SHADER_PATH);
		mDepthShaderProgram.init(DEPTH_VERTEX_SHADER_PATH, DEPTH_FRAGMENT_SHADER_PATH);
		mSceneShaderProgram.use();
		mSceneShaderProgram.setModel(glm::value_ptr(gGlobalRenderState->getRootObject()->getTransform()));
		registerCallbacks();
		mRenderBuffer.init();
		mRenderBuffer.bind();
		initExtraFBO();
		mRendererInitCM.invokeCallbacks();
	}

	void Renderer::registerCallbacks()
	{
		gRootRenderDataStorage->addOnRenderDataUpdatedCallback([this]() {
			mRenderBuffer.loadRenderData(gRootRenderDataStorage->getRenderData());
		});
		gRootRenderDataStorage->addOnExtraRenderDataUpdatedCallback([this]() {
			mExtraRenderBuffer.bind();
			mExtraRenderBuffer.loadRenderData(gRootRenderDataStorage->getExtraRenderData());
			mRenderBuffer.bind();
		});
		gCamera->addOnCameraEditedCallback([this]() {
			mSceneShaderProgram.setView(glm::value_ptr(gCamera->getViewMatrix()));
		});
	}

	void Renderer::initExtraFBO()
	{
		glGenFramebuffers(1, &mExtraFBO);
	}

	void Renderer::renderHighlightedFaces()
	{
		const auto& highlightedFacesData = gGlobalRenderState->getHighlightedFacesData();
		const auto& objectVertexCountMap = Object3D::getObjectVertexCountMap();
		renderOverlayPrimitives(!highlightedFacesData.facesIndices.empty(), HIGHLIGHT_MATERIAL,
			[&highlightedFacesData, &objectVertexCountMap, this]() {
			for (const auto& faceIdx : highlightedFacesData.facesIndices)
			{
				mSceneShaderProgram.setModel(glm::value_ptr(highlightedFacesData.parentObject->getTransform()));
				glDrawArrays(GL_TRIANGLES, faceIdx * 3 + objectVertexCountMap.at(highlightedFacesData.parentObject), 3);
			}
		});
	}

	void Renderer::renderWireframe()
	{
		renderOverlayPrimitives(gGlobalRenderState->getRenderWireframe(), WIREFRAME_MATERIAL, [this]() {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES, 0, gRootRenderDataStorage->getRenderData().getVertexCount());
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		});
	}

	void Renderer::renderWholeObjectHighlighted()
	{
		const auto& objectVertexCountMap = Object3D::getObjectVertexCountMap();
		const auto& highlightedObjectIt = objectVertexCountMap.find(gGlobalRenderState->getHighlightedObject());
		renderOverlayPrimitives(highlightedObjectIt != objectVertexCountMap.end(), HIGHLIGHT_MATERIAL, [this, highlightedObjectIt]() {
			auto& [object, vertexCount] = *highlightedObjectIt;
			mSceneShaderProgram.setModel(glm::value_ptr(object->getTransform()));
			glDrawArrays(GL_TRIANGLES, vertexCount, object->getMesh().getVertices().size());
		});
	}

	void Renderer::renderScene()
	{
		for (auto& [object, vertexCount] : Object3D::getObjectVertexCountMap())
		{
			mSceneShaderProgram.setModel(glm::value_ptr(object->getTransform()));
			glDrawArrays(GL_TRIANGLES, vertexCount, object->getMesh().getVertices().size());
		}
	}

	void Renderer::renderOverlayPrimitives(bool renderCondition, const Material& material, const std::function<void()>& renderFunc)
	{
		if (!renderCondition)
		{
			return;
		}

		mSceneShaderProgram.lighting.setMaterial(material);
		glDepthFunc(GL_LEQUAL);
		renderFunc();
		glDepthFunc(GL_LESS);
		mSceneShaderProgram.lighting.setMaterial(MAIN_MATERIAL);
	}

	void Renderer::renderExtraFBO(const std::function<void()>& renderFunc) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mExtraFBO);
		renderFunc();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		mSceneShaderProgram.setModel(glm::value_ptr(glm::mat4(1.0f)));

		int startIndex = 0;
		for (const auto& extraPrimitive : gGlobalExtraPrimitives->getExtraPrimitives())
		{
			auto vertexCount = extraPrimitive.renderData.getVertexCount();
			mSceneShaderProgram.lighting.setMaterial(extraPrimitive.material);
			glDrawArrays(extraPrimitive.renderMode, startIndex, vertexCount);
			startIndex += vertexCount;
		}

		mSceneShaderProgram.lighting.setMaterial(MAIN_MATERIAL);
		mSceneShaderProgram.setModel(glm::value_ptr(gGlobalRenderState->getRootObject()->getTransform()));
		mRenderBuffer.bind();
	}

	void Renderer::renderShadows()
	{
		renderExtraFBO([]() {
			Texture depthMap(TEXTURE_WIDTH, TEXTURE_HEIGHT, TextureType::DEPTH_MAP);
			depthMap.write([]() {

			});
		});
	}

	void Renderer::adjustLightPos()
	{
		Point3D lightPosInCameraSpace = transformPoint(Point3D(0.0f, LIGHT_SOURCE_POS_Y, FAR_PLANE_DISTANCE), gCamera->getViewMatrix());
		mSceneShaderProgram.lighting.setLightPos(glm::value_ptr(lightPosInCameraSpace));
	}

	RenderBuffer& Renderer::getRenderBuffer()
	{
		return mRenderBuffer;
	}

	SceneShaderProgram& Renderer::getSceneShaderProgram()
	{
		return mSceneShaderProgram;
	}
}
