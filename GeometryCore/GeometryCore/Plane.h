#pragma once

#include <glm/glm.hpp>

#include "Typedefs.h"

namespace GeometryCore
{
	struct Plane
	{
		Point3D origin;
		Vector3D normal;

		Point3D projectPoint(const Point3D& point) const;
		glm::mat4 getTransformToSelf(const Plane& source) const;
	};
}
