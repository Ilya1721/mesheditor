#pragma once

#include <glm/glm.hpp>

namespace GeometryCore
{
	struct Plane
	{
		glm::vec3 projectPoint(const glm::vec3& point) const;

		glm::vec3 origin;
		glm::vec3 normal;

		glm::mat4 getPlaneToPlaneTransform(const Plane& source) const;
	};
}
