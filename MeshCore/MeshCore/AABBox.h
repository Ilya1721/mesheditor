#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "GeometryCore/Typedefs.h"
#include "GeometryCore/Intersectable.h"

namespace GeometryCore
{
	struct Ray;
	struct Plane;
}

namespace MeshCore
{
	using namespace GeometryCore;

	class Mesh;

	class AABBox : public Intersectable
	{
	public:
		AABBox();
		~AABBox();
		AABBox(const AABBox& box) = default;
		AABBox(AABBox&& box) noexcept = default;
		AABBox& operator=(const AABBox& box) = default;
		AABBox& operator=(AABBox&& box) noexcept = default;

		void applyMesh(const Mesh& mesh, const glm::mat4& meshTransform);
		void applyTransform(const glm::mat4& transform);
		void applyOtherBBox(const AABBox& other);
		void clear();

		Point3D getCenter() const;
		const Point3D& getMin() const;
		const Point3D& getMax() const;
		float getHeight() const;
		std::optional<Point3D> findIntersection(const Ray& ray) const override;
		bool isPointInsideBBox(const Point3D& point) const;

	private:
		void calcBBoxPlanes();
		void init();
		void setMinMax(const Point3D& min, const Point3D& max);

	private:
		Point3D mMin;
		Point3D mMax;
		std::vector<Plane> mBBoxPlanes;
	};
}

