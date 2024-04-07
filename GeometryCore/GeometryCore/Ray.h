#pragma once

#include <glm/glm.hpp>
#include <optional>

#include "Typedefs.h"

namespace GeometryCore
{
	struct Plane;

	struct Ray
	{
		Point3D origin;
		Vector3D direction;
	};
}

