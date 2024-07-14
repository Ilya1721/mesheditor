#include "Scene.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "GeometryCore/Ray.h"
#include "GeometryCore/Line.h"
#include "GeometryCore/Transforms.h"
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
		mCamera(&mRenderer.getShaderTransformationSystem()),
		mPickedObject(nullptr),
		mCameraMovementEnabled(true)
	{
		init();
	}

	void Scene::init()
	{
		mRenderer.init(&mRootObject);
		updateRenderBuffer();
	}

	void Scene::setPickedObject(MeshCore::Object3D* pickedObject)
	{
		mPickedObject = pickedObject;
	}

	void Scene::adjustCameraAndLight()
	{
		adjustCamera();
		adjustLightPos();
	}

	void Scene::adjustLightPos()
	{
		Point3D lightPosInCameraSpace = transformPoint(Point3D(0.0f, LIGHT_SOURCE_POS_Y, FAR_PLANE_DISTANCE), mCamera.getViewMatrix());
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

		Point3D cameraPosInCameraSpace = transformPoint(mCamera.getEye(), mCamera.getViewMatrix());
		mRenderer.getLighting().setCameraPos(glm::value_ptr(cameraPosInCameraSpace));
	}

	void Scene::updateRenderBuffer()
	{
		mRenderer.getRenderBuffer().setRenderData(mRootObject.getMesh().getRenderData());
	}

	void Scene::enableCameraMovement(bool isEnabled)
	{
		mCameraMovementEnabled = isEnabled;
	}

	MeshCore::RaySurfaceIntersection Scene::getClosestIntersection(bool intersectSurface)
	{
		const auto& rootBBox = mRootObject.getBBox();
		auto nearCursorPosInWorldSpace = mParentWindow->unProject(mParentWindow->getCursorPos(), 0.0f);
		auto farCursorPosInWorldSpace = mParentWindow->unProject(mParentWindow->getCursorPos(), 1.0f);
		Ray castedRay { nearCursorPosInWorldSpace, farCursorPosInWorldSpace - nearCursorPosInWorldSpace };
		auto transformedCastedRay = glm::translate(rootBBox.getCenter()) * castedRay;
		if (!rootBBox.findIntersection(transformedCastedRay).has_value())
		{
			return {};
		}

		return mRootObject.findIntersection(transformedCastedRay, intersectSurface);
	}

	bool Scene::isCameraMovementEnabled() const
	{
		return mCameraMovementEnabled;
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
