#include "Numeric.h"

#include <cmath>
#include <glm/glm.hpp>

#include "Constants.h"

namespace Geometry
{
	bool isEqual(double first, double second) noexcept
	{
		return std::abs(first - second) < EPSILON;
	}

	double toRadians(double degrees) noexcept
	{
		return glm::radians(degrees);
	}

	double tan(double angle) noexcept
	{
		return std::tan(angle);
	}
}