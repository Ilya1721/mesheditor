#include "Ray.h"

namespace Geometry
{
	Vector3D Ray::findIntersection(const Plane& plane) const
	{
		auto distanceToPlane = (plane.normal * plane.point - plane.normal * point) / (plane.normal * direction);
		return point + distanceToPlane * direction;
	}
}
