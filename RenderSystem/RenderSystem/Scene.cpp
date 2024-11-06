#include "Scene.h"

#include <iostream>

#include "GeometryCore/Ray.h"
#include "GeometryCore/Transforms.h"
#include "MeshFilesLoader/MeshFilesLoader.h"
#include "MeshCore/Intersection.h"
#include "MeshCore/Mesh.h"

#include "Constants.h"
#include "ShadowController.h"
#include "SceneShaderProgram.h"
#include "Renderer.h"

using namespace GeometryCore;

namespace RenderSystem
{
	constexpr Material FLOOR_MATERIAL = PEARL_MATERIAL;
	constexpr Material MAIN_MATERIAL = GOLD_MATERIAL;
	constexpr Material HIGHLIGHT_MATERIAL = RUBY_MATERIAL;
	constexpr Material WIREFRAME_MATERIAL = BLACK_MATERIAL;
}

namespace RenderSystem
{
	Scene::Scene(const std::string& meshFilePath, Renderer* renderer, ShadowController* shadowController, SceneShaderProgram* sceneShaderProgram) :
		mRenderer(renderer),
		mShadowController(shadowController),
		mSceneShaderProgram(sceneShaderProgram),
		mPickedObject(nullptr),
		mRenderWireframe(false),
		mHighlightedObject(nullptr),
		mLightSource(sceneShaderProgram, shadowController)
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
		mSceneShaderProgram->setModel(mRootObject.getTransform());
	}

	void Scene::registerRootObjectCallbacks()
	{
		mRootObject.addOnChildAddedCallback([this](const Object3D* object) {
			onObjectAddedToScene(object);
		});
		mRootObject.addOnObjectUpdatedCallback([this](const Object3D* object, const std::unordered_set<UniqueVertex*>& vertices) {
			onSceneObjectUpdated(object, vertices);
		});
		mRootObject.addOnBBoxUpdatedCallback([this]() {
			onSceneObjectBBoxUpdated();
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

	void Scene::onSceneObjectBBoxUpdated()
	{
		updateLightProjection();
	}

	void Scene::renderScene(Modelable* modelableComponent)
	{
		for (auto& [object, vertexOffset] : mSceneObjectVertexOffsetMap)
		{
			modelableComponent->setModel(object->getTransform());
			mRenderer->renderObject3D(*object, vertexOffset);
		}
	}

	void Scene::renderDepthMap()
	{
		mShadowController->renderSceneToDepthMap([this]() {
			renderScene(mShadowController);
		});
	}

	void Scene::renderSceneDecorations()
	{
		mSceneShaderProgram->setModel(glm::mat4(1.0f));

		int startIndex = 0;
		for (const auto& sceneDecoration : mSceneDecorations)
		{
			mSceneShaderProgram->setMaterial(sceneDecoration.material);
			mRenderer->renderSceneDecoration(sceneDecoration, startIndex);
		}

		mSceneShaderProgram->setMaterial(MAIN_MATERIAL);
		mSceneShaderProgram->setModel(mRootObject.getTransform());
	}

	void Scene::renderHighlightedFaces()
	{
		mSceneShaderProgram->setMaterial(HIGHLIGHT_MATERIAL);

		for (const auto& faceIdx : mHighlightedFacesData.facesIndices)
		{
			mSceneShaderProgram->setModel(mHighlightedFacesData.parentObject->getTransform());
			mRenderer->renderHighlightedFace(faceIdx, mSceneObjectVertexOffsetMap.at(mHighlightedFacesData.parentObject));
		}

		mSceneShaderProgram->setMaterial(MAIN_MATERIAL);
	}

	void Scene::renderWireframe()
	{
		if (!mRenderWireframe)
		{
			return;
		}

		mSceneShaderProgram->setMaterial(WIREFRAME_MATERIAL);

		for (const auto& [object, vertexOffset] : mSceneObjectVertexOffsetMap)
		{
			mSceneShaderProgram->setModel(object->getTransform());
			mRenderer->renderWireframe(object->getVertexCount());
		}

		mSceneShaderProgram->setMaterial(MAIN_MATERIAL);
	}

	void Scene::renderWholeObjectHighlighted()
	{
		const auto& highlightedObjectIt = mSceneObjectVertexOffsetMap.find(mHighlightedObject);
		if (highlightedObjectIt == mSceneObjectVertexOffsetMap.end())
		{
			return;
		}

		mSceneShaderProgram->setMaterial(HIGHLIGHT_MATERIAL);
		const auto& [object, vertexCount] = *highlightedObjectIt;
		mSceneShaderProgram->setModel(object->getTransform());
		mRenderer->renderWholeObjectHighlighted(*object, vertexCount);
		mSceneShaderProgram->setMaterial(MAIN_MATERIAL);
	}

	void Scene::updateLightProjection()
	{
		const auto bboxHeight = mRootObject.getBBox().getHeight();
		float orthoSize = bboxHeight / 2.0f;
		float left = -orthoSize;
		float right = orthoSize;
		float bottom = -orthoSize;
		float top = orthoSize;
		float near_plane = 0.1f;
		float far_plane = bboxHeight * 2;
		const auto& lightProjectionMatrix = glm::ortho(left, right, bottom, top, near_plane, far_plane);

		mShadowController->setLightProjection(lightProjectionMatrix);
		mSceneShaderProgram->setLightProjection(lightProjectionMatrix);
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

	void Scene::toggleWireframe()
	{
		mRenderWireframe = !mRenderWireframe;
	}

	void Scene::highlightWholeObject(const Object3D* object)
	{
		mHighlightedObject = object;
	}

	void Scene::setHighlightedFacesData(const HighlightedFacesData& data)
	{
		mHighlightedFacesData = data;
	}

	void Scene::setLightSourcePos(const Point3D& pos)
	{
		mLightSource.setPosition(pos);
	}

	void Scene::render()
	{
		renderDepthMap();
		mSceneShaderProgram->setDepthMap(0);
		mSceneShaderProgram->invokeAction([this]() {
			mRenderer->cleanScreen();
			renderScene(mSceneShaderProgram);
			renderSceneDecorations();
			renderHighlightedFaces();
			renderWireframe();
			renderWholeObjectHighlighted();
		});
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

	bool Scene::getRenderWireframe()
	{
		return mRenderWireframe;
	}

	const Object3D* Scene::getHighlightedObject()
	{
		return mHighlightedObject;
	}

	const HighlightedFacesData& Scene::getHighlightedFacesData()
	{
		return mHighlightedFacesData;
	}

	const Object3D& Scene::getRootObject() const
	{
		return mRootObject;
	}
}
