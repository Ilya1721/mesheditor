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

using namespace GeometryCore;

namespace RenderSystem
{
	constexpr Material FLOOR_MATERIAL = PEARL_MATERIAL;
}

namespace RenderSystem
{
	Scene::Scene(const std::string& meshFilePath, Renderer* renderer) :
		mPickedObject(nullptr),
		mRenderer(renderer)
	{
		registerRootObjectCallbacks();
		addModelObject(meshFilePath);
	}

	void Scene::addModelObject(const std::string& meshFilePath)
	{
		auto modelObject = std::make_unique<Object3D>(MeshFilesLoader::loadSTL(meshFilePath));
		modelObject->moveToOrigin();
		mRootObject.addChild(std::move(modelObject));
		addSceneDecoration(SceneDecoration::createSceneFloor(mRootObject.getBBox().getHeight(), FLOOR_MATERIAL));
		mRenderer->setModel(glm::value_ptr(mRootObject.getTransform()));
	}

	void Scene::registerRootObjectCallbacks()
	{
		mRootObject.addOnChildAddedCallback([this](const Object3D* object) {
			onObjectAddedToScene(object);
		});
		mRootObject.addOnObjectUpdatedCallback([this](const Object3D* object, const std::unordered_set<UniqueVertex*>& vertices) {
			onSceneObjectUpdated(object, vertices);
		});
	}

	void Scene::onObjectAddedToScene(const Object3D* object)
	{
		mSceneObjectVertexOffsetMap.insert({ object, mSceneRenderData.getVertexCount() });

		for (const auto& vertex : object->getVertices())
		{
			mSceneRenderData.append(vertex);
		}

		mRenderer->loadModelRenderData(mSceneRenderData);
	}

	void Scene::onSceneObjectUpdated(const Object3D* object, const std::unordered_set<UniqueVertex*>& vertices)
	{
		for (auto& vertex : vertices)
		{
			for (auto& originalVertexData : vertex->originalVertices)
			{
				mSceneRenderData.updateVertex(originalVertexData, mSceneObjectVertexOffsetMap.at(object));
			}
		}

		mRenderer->loadModelRenderData(mSceneRenderData);
	}

	void Scene::setPickedObject(Object3D* pickedObject)
	{
		mPickedObject = pickedObject;
	}

	void Scene::addSceneDecoration(const SceneDecoration& sceneDecoration)
	{
		mSceneDecorations.push_back(sceneDecoration);
		mSceneDecorationsRenderData.append(sceneDecoration.renderData);
		mRenderer->loadDecorationsRenderData(mSceneDecorationsRenderData);
	}

	void Scene::render()
	{
		mRenderer->cleanScreen();
		mRenderer->renderScene(mSceneObjectVertexOffsetMap);
		mRenderer->renderHighlightedFaces(mSceneObjectVertexOffsetMap);
		mRenderer->renderWholeObjectHighlighted(mSceneObjectVertexOffsetMap);
		mRenderer->renderWireframe(mSceneRenderData.getVertexCount());
		mRenderer->renderSceneDecorations(mSceneDecorations, mRootObject.getTransform());
	}

	Object3DIntersectionData Scene::getClosestIntersection(const Ray& cursorRay, bool intersectSurface)
	{
		if (mRootObject.getBBox().findIntersectionPoint(cursorRay).has_value())
		{
			return mRootObject.findIntersection(cursorRay, intersectSurface);
		}

		return {};
	}

	Object3D* Scene::getPickedObject() const
	{
		return mPickedObject;
	}

	const std::vector<SceneDecoration>& Scene::getSceneDecorations() const
	{
		return mSceneDecorations;
	}

	const Object3D& Scene::getRootObject() const
	{
		return mRootObject;
	}
}
