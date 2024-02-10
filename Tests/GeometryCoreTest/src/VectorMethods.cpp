#include "TestUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/vector_relational.hpp>

using namespace GeometryCore;

namespace Vec
{
	TEST(Setters, BasicSetters)
	{
		glm::vec2 vec;
		vec.x = testX;
		vec.y = testY;
		EXPECT_TRUE(vec == glm::vec2(testX, testY));
		vec[0] = -1.0f;
		vec[1] = -1.0f;
		EXPECT_TRUE(vec == glm::vec2(-1.0f, -1.0f));
	}

	constexpr float screenWidth = 1920.0f;
	constexpr float screenHeight = 1080.0f;
	constexpr float screenAspect = screenWidth / screenHeight;

	const glm::vec4 viewport = glm::vec4(0.0f, 0.0f, screenWidth, screenHeight);
	const glm::mat4 model = glm::mat4(1.0f);
	const glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	const glm::mat4 projection = glm::perspective(45.0f, screenAspect, 0.01f, 1000.0f);
	const glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 modelView = view;

	TEST(CursorPoint, Project)
	{
		glm::vec4 point4D(1.0f, 2.0f, 3.0f, 1.0f);

		auto pointInView = modelView * point4D;
		auto pointInClip = projection * pointInView;
		auto normalizedPoint = glm::vec3(pointInClip) / pointInClip.w;
		auto screenX = (normalizedPoint.x * 0.5f + 0.5f) * screenWidth;
		auto screenY = (normalizedPoint.y * 0.5f + 0.5f) * screenHeight;
		auto z = normalizedPoint.z * 0.5f + 0.5f;

		auto result = glm::project(glm::vec3(point4D), modelView, projection, viewport);
		EXPECT_FLOAT_EQ(result.x, screenX);
		EXPECT_FLOAT_EQ(result.y, screenY);
		EXPECT_FLOAT_EQ(z, result.z);
	}

	TEST(CursorPoint, UnProject)
	{
		glm::vec3 cursorPoint(200.0f, 400.0f, 0.0f);
		auto xNDC = 2.0f * cursorPoint.x / screenWidth - 1.0f;
		auto yNDC = 2.0f * cursorPoint.y / screenHeight - 1.0f;
		auto zNDC = 2.0f * cursorPoint.z - 1.0f;
		glm::vec4 point4D(xNDC, yNDC, zNDC, 1.0f);
		auto invertedPoint = glm::inverse(modelView) * glm::inverse(projection) * point4D;
		auto result = glm::vec3(invertedPoint) / invertedPoint.w;

		auto expected = glm::unProject(cursorPoint, modelView, projection, viewport);
		EXPECT_TRUE(glm::all(glm::equal(result, expected, 1e-6f)));
	}
}