#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include "AABBox.h"
#include "RenderData.h"
#include "Intersectable.h"
#include "Vertex.h"

namespace MeshCore 
{
	class Object3D : public Intersectable
	{
	public:
		Object3D() = default;
		Object3D(std::unique_ptr<Mesh> mesh);
		Object3D(Object3D&& other) = delete;
		~Object3D();

		Object3D* getParent() const;
		const std::vector<std::unique_ptr<Object3D>>& getChildren() const;
		Mesh& getMesh();
		const Mesh& getMesh() const;
		const glm::mat4& getTransform() const;
		const AABBox& getBBox() const;
		RaySurfaceIntersection findIntersection(const GeometryCore::Ray& ray, bool intersectSurface, int passedFacesCount = 0) const override;
		std::unique_ptr<Object3D> clone(const glm::mat4& initialTransform = glm::mat4(1.0f));

		void addChild(std::unique_ptr<Object3D>&& child);
		void updateTransform(const glm::mat4& transform);
		void onMeshUpdated(const std::unordered_set<UniqueVertex*>& vertices);
		void moveToOrigin();

		static const std::unordered_map<Object3D*, int>& getObjectRenderDataOffsetMap();

	private:
		void init();
		void invokeTransformAction(const std::function<void()>& action, const glm::mat4& transform);
		void propagateRenderDataToRoot();
		void propagateBBoxToRoot() const;

	private:
		Object3D* mParent;
		std::vector<std::unique_ptr<Object3D>> mChildren;
		std::unique_ptr<Mesh> mMesh;
		glm::mat4 mTransform;
		AABBox mBBox;

		static std::unordered_map<Object3D*, int> sObjectRenderDataOffsetMap;
	};
}

