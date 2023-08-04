#include "TestUtils.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Mat
{
	constexpr float left = 0.0f;
	constexpr float right = 1.0f;
	constexpr float bottom = 0.0f;
	constexpr float top = 1.0f;
	constexpr float near = 0.01f;
	constexpr float far = 1000.0f;

	TEST(Projection, Ortho)
	{
		auto orthoMatrixArray = Matrix4DArray
		{ 
			2.0f / (right - left), 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
			0.0f, 0.0f, -2.0f / (far - near), 0.0f,
			(right + left) / (left - right), (top + bottom) / (bottom - top), (far + near) / (near - far), 1.0f
		};

		auto testedOrtho = glm::ortho(left, right, bottom, top, near, far);
		EXPECT_TRUE(matrixEqualMatArray(testedOrtho, orthoMatrixArray));
	}

	constexpr float fov = 0.78539f;
	constexpr float aspect = 1.6f;

	TEST(Projection, Perspective)
	{
		auto perspectiveMatrixArray = Matrix4DArray
		{
			cotangent(fov / 2.0f) / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, cotangent(fov / 2.0f), 0.0f, 0.0f,
			0.0f, 0.0f, (far + near) / (near - far), -1.0f,
			0.0f, 0.0f, 2.0f * far * near / (near - far), 0.0f
		};

		auto testedPerspective = glm::perspective(fov, aspect, near, far);
		EXPECT_TRUE(matrixEqualMatArray(testedPerspective, perspectiveMatrixArray));
	}

	const auto cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	const auto cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	const auto cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
	const auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	const auto cameraDir = glm::normalize(cameraPos - cameraTarget);

	TEST(Matrix4D, LookAt)
	{
		auto rightUpDirMatrix = matFromMatArray(Matrix4DArray
		{
			cameraRight.x, cameraRight.y, cameraRight.z, 0.0f,
			cameraUp.x, cameraUp.y, cameraUp.z, 0.0f,
			cameraDir.x, cameraDir.y, cameraDir.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		});

		auto posMatrix = matFromMatArray(Matrix4DArray
		{
			1.0f, 0.0f, 0.0f, -cameraPos.x,
			0.0f, 1.0f, 0.0f, -cameraPos.y,
			0.0f, 0.0f, 1.0f, -cameraPos.z,
			0.0f, 0.0f, 0.0f, 1.0f,
		});

		auto testedLookAtMat = glm::transpose(rightUpDirMatrix * posMatrix);
		auto lookAtMat = glm::lookAt(cameraPos, cameraTarget, cameraUp);
		EXPECT_TRUE(testedLookAtMat == lookAtMat);
	}
}