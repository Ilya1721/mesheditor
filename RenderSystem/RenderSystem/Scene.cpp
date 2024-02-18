#include "Scene.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "MeshFilesLoader/MeshFilesLoader.h"

#include "Window.h"
#include "Constants.h"

using namespace GeometryCore;

namespace RenderSystem
{
	Scene::Scene(const std::string& meshFilePath, Window* parentWindow) :
		mParentWindow(parentWindow),
		mRootObject(MeshCore::Object3D(nullptr, MeshFilesLoader::loadSTL(meshFilePath)))
	{
		init();
	}

	void Scene::init()
	{
		initRenderBuffer();
		initShaderTransformationSystem();
		adjustCamera();
		adjustLightPos();
	}

	void Scene::initRenderBuffer()
	{
		mRenderer.getRenderBuffer().setRenderData(mRootObject.getRenderData());
	}

	void Scene::initShaderTransformationSystem()
	{
		auto& shaderTransformationSystem = mRenderer.getShaderTransformationSystem();
		shaderTransformationSystem.setViewModel(glm::value_ptr(getViewMatrix()));
		shaderTransformationSystem.setProjection(glm::value_ptr(mParentWindow->getViewport()->getProjectionMatrix()));
	}

	void Scene::render()
	{
		mRenderer.render();
	}

	void Scene::adjustLightPos()
	{
		auto cameraXYVec = (mCamera.getRight() + mCamera.getUp()) * mRootObject.getBBox().getHeight() * 0.25f;
		auto cameraZVec = -mCamera.getNormalizedDirection() * LIGHT_SOURCE_TO_CAMERA_DISTANCE;
		auto lightPos = mCamera.getEye() + cameraXYVec + cameraZVec;
		glm::vec3 lightPosInCameraSpace = mCamera.getViewMatrix() * glm::vec4(lightPos, 1.0f);
		mRenderer.getLighting().setLightPos(glm::value_ptr(lightPosInCameraSpace));
	}

	void Scene::adjustCamera()
	{
		mCamera.adjust(mRootObject.getBBox(), mParentWindow->getViewport()->getFov());
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(mCamera.getViewMatrix()));
		glm::vec3 cameraPosInCameraSpace = mCamera.getViewMatrix() * glm::vec4(mCamera.getEye(), 1.0f);
		mRenderer.getLighting().setCameraPos(glm::value_ptr(cameraPosInCameraSpace));
	}

	void Scene::pan(const glm::vec3& startPointInWorldSpace, const glm::vec3& endPointInWorldSpace)
	{
		mCamera.pan(startPointInWorldSpace, endPointInWorldSpace);
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(mCamera.getViewMatrix()));
	}

	void Scene::orbit(const glm::vec3& startPointInNDC, const glm::vec3& endPointInNDC)
	{
		mCamera.orbit(startPointInNDC, endPointInNDC);
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(mCamera.getViewMatrix()));
	}

	void Scene::zoom(float yOffset)
	{
		mCamera.zoom(yOffset * mRootObject.getBBox().getHeight() * ZOOM_STEP_KOEF);
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(mCamera.getViewMatrix()));
	}

	void Scene::highlightHoveredSurface(const glm::vec3& cursorPosInWorldSpace)
	{
		auto cameraRay = mCamera.getCameraRay(cursorPosInWorldSpace);
		if (!mRootObject.getBBox().checkIntersectionWithRay(cameraRay))
		{
			mRenderer.setHighlightedFaces({});
			return;
		}

		auto faceIntersection = mRootObject.getClosestIntersection(cameraRay, true);
		mRenderer.setHighlightedFaces(faceIntersection.surfaceIndices);
	}

	void Scene::setProjectionMatrix(const glm::mat4& projectionMatrix)
	{
		mRenderer.getShaderTransformationSystem().setProjection(glm::value_ptr(projectionMatrix));
	}

	const glm::mat4& Scene::getViewMatrix() const
	{
		return mCamera.getViewMatrix();
	}
}
