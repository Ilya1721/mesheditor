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

	float toRadians(float degrees)
	{
		return glm::radians(degrees);
	}

	float toDegrees(float radians)
	{
		return glm::degrees(radians);
	}

	float tan(float angle)
	{
		return std::tanf(angle);
	}

	float angle(const Vector3D& first, const Vector3D& second)
	{
		const auto& firstVec = first.getNormalized().__internal_getPimpl()->getVec3();
		const auto& secondVec = second.getNormalized().__internal_getPimpl()->getVec3();

		return glm::angle(firstVec, secondVec);
	}
}