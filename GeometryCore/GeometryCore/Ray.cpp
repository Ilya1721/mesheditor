#include "Ray.h"

namespace Geometry
{
	glm::vec3 Ray::findIntersection(const Plane& plane) const
	{
		auto distanceToPlane = (glm::dot(plane.normal, plane.point) - glm::dot(plane.normal, point)) / glm::dot(plane.normal, direction);
		return point + distanceToPlane * direction;
	}
}
