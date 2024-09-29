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

#include "Window.h"
#include "Viewport.h"
#include "Constants.h"
#include "RenderPrimitive.h"
#include "GlobalExtraPrimitives.h"

using namespace GeometryCore;

namespace RenderSystem
{
	Object3D Scene::sRootObject;
}

namespace RenderSystem
{
	Scene::Scene(const std::string& meshFilePath) :
		mPickedObject(nullptr)
	{
		init(meshFilePath);
	}

	void Scene::init(const std::string& meshFilePath)
	{
		auto firstSceneObject = std::make_unique<Object3D>(MeshFilesLoader::loadSTL(meshFilePath));
		firstSceneObject->moveToOrigin();
		sRootObject.addChild(std::move(firstSceneObject));
		GlobalExtraPrimitives::addSceneFloor();
	}

	void Scene::setPickedObject(Object3D* pickedObject)
	{
		mPickedObject = pickedObject;
	}

	Object3DIntersectionData Scene::getClosestIntersection(const Ray& cursorRay, bool intersectSurface)
	{
		if (sRootObject.getBBox().findIntersectionPoint(cursorRay).has_value())
		{
			return sRootObject.findIntersection(cursorRay, intersectSurface);
		}

		return {};
	}

	Object3D* Scene::getPickedObject() const
	{
		return mPickedObject;
	}

	Object3D& Scene::getRootObject()
	{
		return sRootObject;
	}
}
