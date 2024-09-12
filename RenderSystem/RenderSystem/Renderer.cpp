#include "Renderer.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"
#include "MeshCore/Object3D.h"
#include "MeshCore/Mesh.h"
#include "MeshCore/RootRenderDataStorage.h"

#include "RenderPrimitive.h"
#include "GlobalExtraPrimitives.h"
#include "Scene.h"
#include "GlobalRenderState.h"
#include "Texture.h"
#include "Camera.h"
#include "Window.h"
#include <GeometryCore/Transforms.h>

using namespace MeshCore;

namespace
{
	using namespace RenderSystem;

	constexpr Material MAIN_MATERIAL = GOLD_MATERIAL;
	constexpr Material HIGHLIGHT_MATERIAL = RUBY_MATERIAL;
	constexpr Material WIREFRAME_MATERIAL = BLACK_MATERIAL;

	inline const std::string MAIN_VERTEX_SHADER_PATH = "";
	inline const std::string MAIN_FRAGMENT_SHADER_PATH = "";
	inline const std::string DEPTH_VERTEX_SHADER_PATH = "";
	inline const std::string DEPTH_FRAGMENT_SHADER_PATH = "";

	Camera* gCamera = &Camera::getInstance();
}

namespace RenderSystem
{
	Renderer Renderer::sInstance;
}

namespace RenderSystem
{
	Renderer& Renderer::getInstance()
	{
		return sInstance;
	}

	Renderer::Renderer() :
		mExtraFBO(),
		mRenderBuffer(),
		mExtraRenderBuffer(),
		mSceneShaderProgram(MAIN_VERTEX_SHADER_PATH, MAIN_FRAGMENT_SHADER_PATH),
		mDepthShaderProgram(DEPTH_VERTEX_SHADER_PATH, DEPTH_FRAGMENT_SHADER_PATH)
	{}

	Renderer::~Renderer()
	{
		glDeleteFramebuffers(1, &mExtraFBO);
	}

	void Renderer::init()
	{
		mSceneShaderProgram.use();
		mSceneShaderProgram.setModel(glm::value_ptr(Scene::getRootObject().getTransform()));
		registerCallbacks();
		mRenderBuffer.bind();
		initExtraFBO();
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
		const auto& highlightedFacesData = GlobalRenderState::getHighlightedFacesData();
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
		for (const auto& extraPrimitive : GlobalExtraPrimitives::getExtraPrimitives())
		{
			auto vertexCount = extraPrimitive.renderData.getVertexCount();
			mSceneShaderProgram.lighting.setMaterial(extraPrimitive.material);
			glDrawArrays(extraPrimitive.renderMode, startIndex, vertexCount);
			startIndex += vertexCount;
		}

		mSceneShaderProgram.lighting.setMaterial(MAIN_MATERIAL);
		mSceneShaderProgram.setModel(glm::value_ptr(Scene::getRootObject().getTransform()));
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
}
