#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include "MeshCore/AABBox.h"
#include "MeshCore/Vertex.h"

#include "RenderData.h"
#include "Object3DIntersectionData.h"

using namespace MeshCore;

namespace RenderSystem
{
	class Object3D
	{
	public:
		Object3D() = default;
		Object3D(std::unique_ptr<Mesh> mesh);
		Object3D(Object3D&& other) = delete;
		~Object3D();

		Object3D* getParent() const;
		const std::vector<std::unique_ptr<Object3D>>& getChildren() const;
		const Mesh& getMesh() const;
		const glm::mat4& getTransform() const;
		const AABBox& getBBox() const;
		const RenderData& getRenderData() const;
		Object3DIntersectionData findIntersection(const GeometryCore::Ray& ray, bool intersectSurface, int passedFacesCount = 0);
		std::unique_ptr<Object3D> clone();

		void addChild(std::unique_ptr<Object3D>&& child);
		void updateTransform(const glm::mat4& transform);
		void onMeshUpdated(const std::unordered_set<UniqueVertex*>& vertices);
		void moveToOrigin();

		static const std::unordered_map<const Object3D*, int>& getObjectVertexCountMap();

	private:
		void init();
		void prepareRenderData();
		void invokeTransformAction(const std::function<void()>& action, const glm::mat4& transform);
		void propagateRenderDataToRoot();
		void propagateBBoxToRoot() const;

	private:
		Object3D* mParent;
		std::vector<std::unique_ptr<Object3D>> mChildren;
		std::unique_ptr<Mesh> mMesh;
		glm::mat4 mTransform;
		AABBox mBBox;
		RenderData mRenderData;

		static std::unordered_map<const Object3D*, int> sObjectVertexCountMap;
	};
}
