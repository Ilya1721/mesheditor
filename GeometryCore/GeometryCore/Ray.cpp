#include "Ray.h"

namespace GeometryCore
{
	Ray operator*(const glm::mat4& transform, const Ray& ray)
	{
		Point3D transformedOrigin = transform * Point4D(ray.origin, 1.0f);
		Point3D transformedDirection = transform * Point4D(ray.direction, 0.0f);

		return { transformedOrigin, transformedDirection };
	}
}