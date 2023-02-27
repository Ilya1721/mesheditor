#pragma once

#include <glm/glm.hpp>

#include "Utility/SolutionMacros.h"

#include "Object3D.h"

namespace MeshCore
{
	class AABBox
	{
	public:
		API AABBox() noexcept;
		API AABBox(const AABBox& box) = default;
		API AABBox(AABBox&& box) = default;
		API AABBox& operator=(const AABBox& box) = default;
		API AABBox& operator=(AABBox&& box) = default;
		API virtual ~AABBox() noexcept = default;

		API void setFromMesh(const Mesh& mesh) noexcept;
		API void setFromObject(const Object3D& object) noexcept;

		API void applyTransform(const glm::mat4& transform) noexcept;

		API glm::vec3 getCenter() const noexcept;
		API const glm::vec3& getMin() const noexcept;
		API const glm::vec3& getMax() const noexcept;
		API double getHeight() const noexcept;

	private:
		void init() noexcept;

	private:
		glm::vec4 mMin;
		glm::vec4 mMax;
	};
}

