#include "TestUtils.h"

#include "GeometryCore/Ray.h"

namespace Ray
{
	TEST(Methods, FindIntersection)
	{
		GeometryCore::Ray ray(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		Plane plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		EXPECT_TRUE(ray.findIntersection(plane) == glm::vec3(0.0f, 0.0f, 0.0f));
	}
}