#include "Scene.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "GeometryCore/Ray.h"
#include "GeometryCore/Line.h"
#include "MeshFilesLoader/MeshFilesLoader.h"
#include "MeshCore/Intersection.h"
#include "MeshCore/Mesh.h"

#include "Window.h"
#include "Viewport.h"
#include "Constants.h"
#include "RenderPrimitive.h"

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

	void Scene::initShaderTransformationSystem()
	{
		auto& shaderTransformationSystem = mRenderer.getShaderTransformationSystem();
		shaderTransformationSystem.setViewModel(glm::value_ptr(getViewMatrix()));
		shaderTransformationSystem.setProjection(glm::value_ptr(mParentWindow->getViewport()->getProjectionMatrix()));
	}

	void Scene::initRenderBuffer()
	{
		mRenderer.getRenderBuffer().bind();
		updateRenderData();
	}

	void Scene::render()
	{
		mRenderer.render();
		mRenderer.renderDebug();
	}

	void Scene::toggleWireframe()
	{
		mRenderer.toggleWireframe();
	}

	void Scene::adjustLightPos()
	{
		auto cameraXYVec = (mCamera.getRight() + mCamera.getUp()) * mRootObject.getBBox().getHeight() * 0.25f;
		auto cameraZVec = -mCamera.getNormalizedDirection() * LIGHT_SOURCE_TO_CAMERA_DISTANCE;
		auto lightPos = mCamera.getEye() + cameraXYVec + cameraZVec;
		Point3D lightPosInCameraSpace = mCamera.getViewMatrix() * Point4D(lightPos, 1.0f);
		mRenderer.getLighting().setLightPos(glm::value_ptr(lightPosInCameraSpace));
	}

	void Scene::adjustCamera()
	{
		mCamera.adjust(mRootObject.getBBox(), mParentWindow->getViewport()->getFov());
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(mCamera.getViewMatrix()));
		Point3D cameraPosInCameraSpace = mCamera.getViewMatrix() * Point4D(mCamera.getEye(), 1.0f);
		mRenderer.getLighting().setCameraPos(glm::value_ptr(cameraPosInCameraSpace));
	}

	void Scene::pan(const Point3D& startPointInWorldSpace, const Point3D& endPointInWorldSpace)
	{
		mCamera.pan(startPointInWorldSpace, endPointInWorldSpace);
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(mCamera.getViewMatrix()));
	}

	void Scene::orbit(const Point3D& startPointInNDC, const Point3D& endPointInNDC)
	{
		mCamera.orbit(startPointInNDC, endPointInNDC);
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(mCamera.getViewMatrix()));
	}

	void Scene::zoom(float yOffset)
	{
		mCamera.zoom(yOffset * mRootObject.getBBox().getHeight() * ZOOM_STEP_KOEF);
		mRenderer.getShaderTransformationSystem().setViewModel(glm::value_ptr(mCamera.getViewMatrix()));
	}

	void Scene::highlightFaces(const std::vector<int>& facesIndices)
	{
		mRenderer.setHighlightedFaces(facesIndices);
	}

	void Scene::updateRenderData()
	{
		mRenderer.getRenderBuffer().setRenderData(mRootObject.getOnlyRootRenderData());
	}

	MeshCore::RaySurfaceIntersection Scene::getClosestIntersection(const Point3D& cursorPosInWorldSpace, bool intersectSurface)
	{
		auto cameraRay = mCamera.getCameraRay(cursorPosInWorldSpace);
		if (!mRootObject.getBBox().findIntersection(cameraRay).has_value())
		{
			return {};
		}

		return mRootObject.findIntersection(cameraRay, intersectSurface);
	}

	void Scene::setProjectionMatrix(const glm::mat4& projectionMatrix)
	{
		mRenderer.getShaderTransformationSystem().setProjection(glm::value_ptr(projectionMatrix));
	}

	const Camera& Scene::getCamera() const
	{
		return mCamera;
	}

	const glm::mat4& Scene::getViewMatrix() const
	{
		return mCamera.getViewMatrix();
	}

	Window* Scene::getParentWindow() const
	{
		return mParentWindow;
	}
}
