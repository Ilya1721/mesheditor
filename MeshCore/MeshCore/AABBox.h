#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace GeometryCore
{
	struct Ray;
	struct Plane;
}

namespace MeshCore
{
	class Mesh;

	class AABBox
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

		glm::vec3 getCenter() const;
		const glm::vec3& getMin() const;
		const glm::vec3& getMax() const;
		float getHeight() const;
		bool checkIntersectionWithRay(const GeometryCore::Ray& ray) const;
		bool isPointInsideBBox(const glm::vec3& point) const;

	private:
		void calcBBoxPlanes();
		void init();
		void setMinMax(const glm::vec3& min, const glm::vec3& max);

	private:
		glm::vec3 mMin;
		glm::vec3 mMax;
		std::vector<GeometryCore::Plane> mBBoxPlanes;
	};
}

