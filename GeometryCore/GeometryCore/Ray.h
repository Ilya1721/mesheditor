#pragma once

#include "Plane.h"

namespace Geometry
{
	struct Ray
	{
		Vector3D point;
		Vector3D direction;

		Vector3D findIntersection(const Plane& plane) const;
	};
}

