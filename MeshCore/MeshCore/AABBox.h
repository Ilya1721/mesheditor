#pragma once

#include <glm/glm.hpp>

#include "Object3D.h"

namespace MeshCore
{
	class AABBox final
	{
	public:
		AABBox();
		AABBox(const AABBox& box) = default;
		AABBox(AABBox&& box) noexcept = default;
		AABBox& operator=(const AABBox& box) = default;
		AABBox& operator=(AABBox&& box) noexcept = default;

		void setFromMesh(const Mesh& mesh);
		void setFromObject(const Object3D& object);

		void applyTransform(const glm::mat4& transform);

		glm::vec3 getCenter() const;
		const glm::vec3& getMin() const;
		const glm::vec3& getMax() const;
		float getHeight() const;

	private:
		void init();

	private:
		glm::vec3 mMin;
		glm::vec3 mMax;
	};
}

