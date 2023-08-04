#pragma once

#include "Plane.h"

namespace Geometry
{
	struct Ray
	{
		glm::vec3 point;
		glm::vec3 direction;

		glm::vec3 findIntersection(const Plane& plane) const;
	};
}

