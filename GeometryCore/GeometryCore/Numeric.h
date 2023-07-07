#pragma once

#include "Vector.h"

namespace Geometry
{
	bool isEqual(float first, float second);
	float toRadians(float degrees);
	float toDegrees(float radians);
	float tan(float angle);
	float angle(const Vector3D& first, const Vector3D& second);
}
