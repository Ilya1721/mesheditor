#pragma once

#include <memory>
#include <string>
#include <functional>

#include "GeometryCore/Typedefs.h"

#include "Object3D.h"
#include "Object3DIntersectionData.h"
#include "SceneDecoration.h"

namespace RenderSystem
{
	using namespace GeometryCore;

	class Renderer;

	class Scene
	{
	public:
		Scene(const std::string& meshFilePath, Renderer* renderer);
		Scene(Scene&& scene) = delete;

		Object3DIntersectionData getClosestIntersection(const Ray& cursorRay, bool intersectSurface);
		Object3D* getPickedObject() const;
		const std::vector<SceneDecoration>& getSceneDecorations() const;

		void setPickedObject(Object3D* pickedObject);
		void addSceneDecoration(const SceneDecoration& sceneDecoration);
		void render();

		const Object3D& getRootObject() const;

	private:
		void addModelObject(const std::string& meshFilePath);
		void registerRootObjectCallbacks();
		void onObjectAddedToScene(const Object3D* object);
		void onSceneObjectUpdated(const Object3D* object, const std::unordered_set<UniqueVertex*>& vertices);

	private:
		Object3D* mPickedObject;
		Renderer* mRenderer;

		std::vector<SceneDecoration> mSceneDecorations;
		RenderData mSceneDecorationsRenderData;

		RenderData mSceneRenderData;
		std::unordered_map<const Object3D*, int> mSceneObjectVertexOffsetMap;
		Object3D mRootObject;
	};
}