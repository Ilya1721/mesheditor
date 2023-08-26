#include <gtest/gtest.h>

#include <glm/gtc/matrix_transform.hpp>

#include "TestUtils.h"

#include "MeshCore/AABBox.h"

namespace AABBox
{
	TEST(Methods, SetFromMesh)
	{
		auto cube = loadCube();
		MeshCore::AABBox bbox;
		bbox.setFromMesh(cube, glm::mat4(1.0f));
		const auto& bboxMin = bbox.getMin();
		const auto& bboxMax = bbox.getMax();
		EXPECT_TRUE(bboxMin == glm::vec3(-55.0f, 40.0f, 0.0f));
		EXPECT_TRUE(bboxMax == glm::vec3(-35.0f, 60.0f, 20.0f));
		EXPECT_TRUE(bbox.getCenter() == (bboxMin + bboxMax) * 0.5f);
		EXPECT_TRUE(bbox.getHeight() == bboxMax.y - bboxMin.y);
	}

	TEST(Methods, SetFromObject)
	{
		auto cube = loadCube();
		MeshCore::Object3D object(nullptr, cube);
		auto cubeVertices = cube.getVertices();
		auto translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));
		std::vector<MeshCore::Vertex> transformedVertices;
		std::transform(cubeVertices.begin(), cubeVertices.end(), std::back_inserter(transformedVertices),
			[translateMatrix](const MeshCore::Vertex& vertex) 
			{ return MeshCore::Vertex(translateMatrix * glm::vec4(vertex.pos, 1.0f), vertex.normal); });
		MeshCore::Mesh transformedMesh(transformedVertices);
		MeshCore::Object3D childObject(&object, transformedMesh);

		MeshCore::AABBox bbox;
		bbox.setFromObject(object);
		const auto& bboxMin = bbox.getMin();
		const auto& bboxMax = bbox.getMax();
		EXPECT_TRUE(bboxMin == glm::vec3(-55.0f, 40.0f, 0.0f));
		EXPECT_TRUE(bboxMax == glm::vec3(-30.0f, 65.0f, 25.0f));
	}
}