#pragma once

#include <glm/glm.hpp>
#include <optional>

namespace GeometryCore
{
	struct Plane;

	struct Ray
	{
		glm::vec3 origin;
		glm::vec3 direction;

		std::optional<glm::vec3> findIntersection(const Plane& plane) const;
	};
}

