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
#include "GlobalExtraPrimitives.h"

using namespace GeometryCore;

namespace RenderSystem
{
	Scene Scene::sInstance;
}

namespace RenderSystem
{
	Scene& Scene::getInstance()
	{
		return sInstance;
	}

	void Scene::init(const std::string& meshFilePath)
	{
		auto firstSceneObject = std::make_unique<MeshCore::Object3D>(MeshFilesLoader::loadSTL(meshFilePath));
		firstSceneObject->moveToOrigin();
		sRootObject.addChild(std::move(firstSceneObject));
		GlobalExtraPrimitives::addSceneFloor();
	}

	void Scene::adjustLightPos()
	{
		Point3D lightPosInCameraSpace = transformPoint(Point3D(0.0f, LIGHT_SOURCE_POS_Y, FAR_PLANE_DISTANCE), mCamera.getViewMatrix());
		mRenderer.getLighting().setLightPos(glm::value_ptr(lightPosInCameraSpace));
	}

	const MeshCore::Object3D& Scene::getRootObject() const
	{
		return sRootObject;
	}
}
