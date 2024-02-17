#pragma once

#include <optional>

#include "Plane.h"

namespace GeometryCore
{
	struct Ray
	{
		glm::vec3 origin;
		glm::vec3 direction;

		std::optional<glm::vec3> findIntersection(const Plane& plane) const;
	};
}

