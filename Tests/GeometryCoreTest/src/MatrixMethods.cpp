#include "TestUtils.h"

namespace Methods
{
	TEST(Matrix4D, GetInverse)
	{
		Matrix4D scaleMatrix(scaleFactor);
		auto inverse = scaleMatrix.getInverse();
		EXPECT_MAT_EQ(inverse * scaleMatrix, identityMatrix);
		EXPECT_MAT_EQ(scaleMatrix * inverse, identityMatrix);
	}

	TEST(Matrix4D, GetScaleMatrix)
	{
		auto scaleMatrix = Matrix4D::getScaleMatrix(Vector3D{ scaleFactor, scaleFactor, scaleFactor });
		Vector3D vec3(1.0f, 1.0f, 1.0f);
		auto result = scaleMatrix * vec3;
		EXPECT_VEC_EQ(result, Vector3D{ 2.0f, 2.0f, 2.0f });
	}

	TEST(Matrix4D, GetRotationMatrix)
	{
		auto rotationMatrix = Matrix4D::getRotationMatrix(HALF_PI, Vector3D{ 0.0f, 0.0f, 1.0f });
		Vector3D xAxis(1.0f, 0.0f, 0.0f);
		auto result = rotationMatrix * xAxis;
		EXPECT_VEC_EQ(result, Vector3D{ 0.0f, 1.0f, 0.0f });
	}

	namespace Projection
	{
		constexpr float left = 0.0f;
		constexpr float right = 1.0f;
		constexpr float bottom = 0.0f;
		constexpr float top = 1.0f;
		constexpr float near = 0.01f;
		constexpr float far = 1000.0f;
		constexpr float fov = 0.78539f;
		constexpr float aspect = 1.6f;

		TEST(Matrix4D, Ortho)
		{
			auto orthoMatrixArray = Matrix4DArray
			{ 
			  2.0f / (right - left), 0.0f, 0.0f, 0.0f,
			  0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
			  0.0f, 0.0f, -2.0f / (far - near), 0.0f,
			  (right + left) / (left - right), (top + bottom) / (bottom - top), (far + near) / (near - far), 1.0f
			};

			auto testedOrtho = Matrix4D::ortho(left, right, bottom, top, near, far);
			EXPECT_MAT_EQ(testedOrtho, orthoMatrixArray);
		}

		TEST(Matrix4D, Perspective)
		{
			auto perspectiveMatrixArray = Matrix4DArray
			{
			  cotangent(fov / 2.0f) / aspect, 0.0f, 0.0f, 0.0f,
			  0.0f, cotangent(fov / 2.0f), 0.0f, 0.0f,
			  0.0f, 0.0f, (far + near) / (near - far), -1.0f,
			  0.0f, 0.0f, 2.0f * far * near / (near - far), 0.0f
			};

			auto testedPerspective = Matrix4D::perspective(fov, aspect, near, far);
			EXPECT_MAT_EQ(testedPerspective, perspectiveMatrixArray);
		}
	}
}