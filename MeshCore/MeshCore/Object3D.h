#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include "GeometryCore/Intersectable.h"

#include "AABBox.h"
#include "RenderData.h"

namespace GeometryCore
{
	struct Ray;
}

namespace MeshCore 
{
	struct RaySurfaceIntersection;

	class Object3D : public ModifiableIntersectable
	{
	public:
		Object3D() = default;
		Object3D(std::unique_ptr<Mesh> mesh);
		Object3D(Object3D&& other) = delete;
		~Object3D();

		Object3D* getParent() const;
		const std::vector<std::unique_ptr<Object3D>>& getChildren() const;
		Mesh& getMesh();
		const RenderData& getRenderData() const;
		const glm::mat4& getTransform() const;
		const AABBox& getBBox() const;
		std::optional<Point3D> findIntersection(const Ray& ray) override;
		RaySurfaceIntersection findIntersection(const GeometryCore::Ray& ray, bool intersectSurface, int passedFacesCount = 0);
		std::unique_ptr<Object3D> clone(const glm::mat4& initialTransform = glm::mat4(1.0f));

		void addChild(std::unique_ptr<Object3D>&& child);
		void setTransform(const glm::mat4& transform);
		void updateTransform(const glm::mat4& transform);
		void updateRenderData(const OriginalVertexData& vertexData);
		void updateBBox();
		void calculateBBox();
		void moveToOrigin();

		static const std::unordered_map<Object3D*, int>& getObjectRenderDataOffsetMap();

	private:
		void init();
		void setParent(Object3D* parent);
		void invokeTransformAction(const std::function<void()>& action, const glm::mat4& transform);
		void propagateRenderDataToRoot();
		void propagateBBoxToRoot() const;
		Object3D* getRoot();

	private:
		Object3D* mParent;
		std::vector<std::unique_ptr<Object3D>> mChildren;
		std::unique_ptr<Mesh> mMesh;
		glm::mat4 mTransform;
		AABBox mBBox;
		RenderData mRenderData;

		static std::unordered_map<Object3D*, int> sObjectRenderDataOffsetMap;
	};
}

