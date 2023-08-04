#include "Numeric.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Constants.h"
#include "VecImpl.h"

namespace Geometry
{
	bool isEqual(float first, float second)
	{
		return std::abs(first - second) < EPSILON;
	}
}