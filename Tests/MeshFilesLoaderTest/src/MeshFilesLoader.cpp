#include <gtest/gtest.h>

#include "TestUtils.h"

namespace MeshFilesLoader
{
	TEST(Methods, LoadSTL)
	{
		auto cube = loadCube();
		const auto& cubeVertices = cube.getVertices();
		EXPECT_TRUE(cubeVertices[0] == MeshCore::Vertex(glm::vec3(-35.0f, 60.0f, 20.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		EXPECT_TRUE(cubeVertices[1] == MeshCore::Vertex(glm::vec3(-55.0f, 60.0f, 20.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		EXPECT_TRUE(cubeVertices[2] == MeshCore::Vertex(glm::vec3(-35.0f, 40.0f, 20.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	}
}
