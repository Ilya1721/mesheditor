#include "Numeric.h"

#include <cmath>
#include <glm/glm.hpp>

#include "Constants.h"

namespace Geometry
{
	bool isEqual(float first, float second)
	{
		return std::abs(first - second) < EPSILON;
	}

	float toRadians(float degrees)
	{
		return glm::radians(degrees);
	}

	float tan(float angle)
	{
		return std::tanf(angle);
	}
}