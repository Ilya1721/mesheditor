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
#include "MeshCore/RenderData.h"

#include "Window.h"
#include "Viewport.h"
#include "Constants.h"
#include "RenderPrimitive.h"

using namespace GeometryCore;

namespace RenderSystem
{
	Scene::Scene(const std::string& meshFilePath, Window* parentWindow) :
		mParentWindow(parentWindow),
		mRootObject(MeshCore::Object3D(nullptr, MeshFilesLoader::loadSTL(meshFilePath))),
		mCamera(&mRenderer.getShaderTransformationSystem())
	{
		init();
	}

	void Scene::init()
	{
		mRenderer.init(&mRootObject);
		updateRenderBuffer();
	}

	void Scene::adjustCameraAndLight()
	{
		adjustCamera();
		adjustLightPos();
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
		const auto& viewport = mParentWindow->getViewport();
		if (viewport->getProjectionType() == PROJECTION_TYPE::PERSPECTIVE)
		{
			mCamera.perspectiveAdjust(mRootObject.getBBox(), viewport->getFov());
		}
		else
		{
			mCamera.orthoAdjust(mRootObject.getBBox());
		}

		Point3D cameraPosInCameraSpace = mCamera.getViewMatrix() * Point4D(mCamera.getEye(), 1.0f);
		mRenderer.getLighting().setCameraPos(glm::value_ptr(cameraPosInCameraSpace));
	}

	void Scene::updateRenderBuffer()
	{
		mRenderer.getRenderBuffer().setRenderData(mRootObject.getMesh().getRenderData());
	}

	MeshCore::RaySurfaceIntersection Scene::getClosestIntersection(bool intersectSurface)
	{
		auto nearCursorPosInWorldSpace = mParentWindow->unProject(mParentWindow->getCursorPos(), 0.0f);
		auto farCursorPosInWorldSpace = mParentWindow->unProject(mParentWindow->getCursorPos(), 1.0f);
		Ray castedRay { nearCursorPosInWorldSpace, farCursorPosInWorldSpace - nearCursorPosInWorldSpace };
		if (!mRootObject.getBBox().findIntersection(castedRay).has_value())
		{
			return {};
		}

		return mRootObject.findIntersection(castedRay, intersectSurface);
	}

	Camera& Scene::getCamera()
	{
		return mCamera;
	}

	Window* Scene::getParentWindow()
	{
		return mParentWindow;
	}

	Renderer& Scene::getRenderer()
	{
		return mRenderer;
	}

	MeshCore::Object3D& Scene::getRootObject()
	{
		return mRootObject;
	}
}
