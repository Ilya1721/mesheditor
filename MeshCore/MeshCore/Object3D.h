#pragma once

#include <memory>
#include <unordered_set>

#include "GeometryCore/Intersectable.h"

#include "AABBox.h"

namespace GeometryCore
{
	struct Ray;
}

namespace MeshCore 
{
	class Mesh;
	struct RaySurfaceIntersection;
	struct RenderData;

	class Object3D : public ModifiableIntersectable
	{
	public:
		Object3D();
		~Object3D();
		Object3D(Object3D* parent, std::unique_ptr<Mesh> mesh);
		Object3D(const Object3D& other) = delete;
		Object3D(Object3D&& other) noexcept = default;
		Object3D& operator=(const Object3D& other) = delete;
		Object3D& operator=(Object3D&& other) noexcept = default;

		bool operator==(const Object3D& other) const = default;

		void init();
		Object3D* getParent() const;
		const std::unordered_set<Object3D*>& getChildren() const;
		void appendChild(Object3D* child);
		void removeChild(Object3D* child);

		const Mesh& getMesh() const;
		Mesh& getMesh();
		const RenderData& getOnlyRootRenderData() const;
		RenderData getRenderData();
		const glm::mat4& getTransform() const;
		void setTransform(const glm::mat4& transform);
		const AABBox& getBBox() const;
		std::optional<Point3D> findIntersection(const Ray& ray) override;
		RaySurfaceIntersection findIntersection(const GeometryCore::Ray& ray, bool intersectSurface, int passedFacesCount = 0);

	private:
		void calculateBBox(const Object3D* object);
		void setParent(Object3D* parent);
		void updateParentBBox(Object3D* parent) const;
		void recalcParentBBox(Object3D* parent) const;

	private:
		Object3D* mParent;
		std::unordered_set<Object3D*> mChildren;
		std::unique_ptr<Mesh> mMesh;
		glm::mat4 mTransform;
		AABBox mBBox;
	};
}

