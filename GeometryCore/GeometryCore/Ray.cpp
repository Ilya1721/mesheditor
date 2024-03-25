#include "Ray.h"

#include <glm/gtc/epsilon.hpp>

#include "Numeric.h"
#include "Plane.h"

namespace GeometryCore
{
	std::optional<glm::vec3> Ray::findIntersection(const Plane& plane) const
	{
		if (glm::epsilonEqual(glm::dot(plane.normal, direction), 0.0f, 1e-6f))
		{
			return {};
		}

		auto distanceToPlane = (glm::dot(plane.normal, plane.origin) - glm::dot(plane.normal, origin)) / glm::dot(plane.normal, direction);

		return std::make_optional(origin + distanceToPlane * direction);
	}
}
