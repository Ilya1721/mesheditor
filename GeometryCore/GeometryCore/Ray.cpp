#include "Ray.h"

#include "Numeric.h"

namespace GeometryCore
{
	std::optional<glm::vec3> Ray::findIntersection(const Plane& plane) const
	{
		if (isEqual(glm::dot(plane.normal, direction), 0.0f))
		{
			return {};
		}

		auto distanceToPlane = (glm::dot(plane.normal, plane.point) - glm::dot(plane.normal, point)) / glm::dot(plane.normal, direction);

		return std::make_optional(point + distanceToPlane * direction);
	}
}
