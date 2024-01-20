#include <gtest/gtest.h>

#include "TestUtils.h"

namespace Mesh
{
	TEST(Constructors, VerticesConstructor)
	{
		std::vector<MeshCore::Vertex> vertices;
		vertices.push_back(defaultVertex);
		MeshCore::Mesh mesh(vertices);
		EXPECT_TRUE(vertices[0] == mesh.getVertices()[0]);
	}

	TEST(Methods, GetRenderData)
	{
		auto mesh = getDefaultMesh();
		auto renderData = mesh.getRenderData();
		for (glm::length_t vecIdx = 0; vecIdx < 3; ++vecIdx)
		{
			EXPECT_FLOAT_EQ(defaultVertex.pos[vecIdx], renderData.positions[vecIdx]);
			EXPECT_FLOAT_EQ(defaultVertex.normal[vecIdx], renderData.normals[vecIdx]);
		}
	}
}