#include "Numeric.h"

#include <cmath>
#include <glm/glm.hpp>

#include "Constants.h"

namespace Geometry
{
	bool isEqual(float first, float second) noexcept
	{
		return std::abs(first - second) < EPSILON;
	}

	float toRadians(float degrees) noexcept
	{
		return glm::radians(degrees);
	}

	float tan(float angle) noexcept
	{
		return std::tan(angle);
	}
}