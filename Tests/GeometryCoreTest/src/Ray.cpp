#include "TestUtils.h"

#include "GeometryCore/Ray.h"

namespace Methods
{
	TEST(Ray, FindIntersection)
	{
		Ray ray(Vector3D{ 0.0f, 0.0f, 1.0f }, Vector3D{ 0.0f, 0.0f, -1.0f });
		Plane plane(Vector3D{ 0.0f, 0.0f, 0.0f }, Vector3D{ 0.0f, 0.0f, 1.0f });
		EXPECT_TRUE(ray.findIntersection(plane) == (Vector3D{ 0.0f, 0.0f, 0.0f }));
	}
}