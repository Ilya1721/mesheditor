#include "TestUtils.h"

namespace Methods
{
	TEST(Matrix4D, GetInverse)
	{
		Matrix4D scaleMatrix(scaleFactor);
		auto inverse = scaleMatrix.getInverse();
		EXPECT_TRUE((inverse * scaleMatrix) == identityMatrix);
		EXPECT_TRUE((scaleMatrix * inverse) == identityMatrix);
	}

	TEST(Matrix4D, GetTransposed)
	{
		auto rotationMatrix = Matrix4D::getRotationMatrix(HALF_PI, Vector3D{ 0.0f, 0.0f, 1.0f });
		auto transposedRotMatrix = rotationMatrix.getTransposed();
		EXPECT_TRUE(transposedRotMatrix.getTransposed() == rotationMatrix);
	}

	TEST(Matrix4D, GetScaleMatrix)
	{
		auto scaleMatrix = Matrix4D::getScaleMatrix(Vector3D{ scaleFactor, scaleFactor, scaleFactor });
		Vector3D vec3(1.0f, 1.0f, 1.0f);
		auto result = scaleMatrix * vec3;
		EXPECT_TRUE(result == (Vector3D{ 2.0f, 2.0f, 2.0f }));
	}

	TEST(Matrix4D, GetRotationMatrix)
	{
		auto rotationMatrix = Matrix4D::getRotationMatrix(HALF_PI, Vector3D{ 0.0f, 0.0f, 1.0f });
		Vector3D xAxis(1.0f, 0.0f, 0.0f);
		auto result = rotationMatrix * xAxis;
		EXPECT_TRUE(result == (Vector3D{ 0.0f, 1.0f, 0.0f }));
	}

	namespace Projection
	{
		constexpr float left = 0.0f;
		constexpr float right = 1.0f;
		constexpr float bottom = 0.0f;
		constexpr float top = 1.0f;
		constexpr float near = 0.01f;
		constexpr float far = 1000.0f;

		TEST(Matrix4D, Ortho)
		{
			auto orthoMatrixArray = Matrix4D(Matrix4DArray
			{ 
				2.0f / (right - left), 0.0f, 0.0f, 0.0f,
				0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
				0.0f, 0.0f, -2.0f / (far - near), 0.0f,
				(right + left) / (left - right), (top + bottom) / (bottom - top), (far + near) / (near - far), 1.0f
			}.data());

			auto testedOrtho = Matrix4D::ortho(left, right, bottom, top, near, far);
			EXPECT_TRUE(testedOrtho == orthoMatrixArray);
		}

		constexpr float fov = 0.78539f;
		constexpr float aspect = 1.6f;

		TEST(Matrix4D, Perspective)
		{
			auto perspectiveMatrixArray = Matrix4D(Matrix4DArray
			{
				cotangent(fov / 2.0f) / aspect, 0.0f, 0.0f, 0.0f,
				0.0f, cotangent(fov / 2.0f), 0.0f, 0.0f,
				0.0f, 0.0f, (far + near) / (near - far), -1.0f,
				0.0f, 0.0f, 2.0f * far * near / (near - far), 0.0f
			}.data());

			auto testedPerspective = Matrix4D::perspective(fov, aspect, near, far);
			EXPECT_TRUE(testedPerspective == perspectiveMatrixArray);
		}

		const auto cameraPos = Vector3D(0.0f, 0.0f, 1.0f);
		const auto cameraTarget = Vector3D(0.0f, 0.0f, 0.0f);
		const auto cameraRight = Vector3D(1.0f, 0.0f, 0.0f);
		const auto cameraUp = Vector3D(0.0f, 1.0f, 0.0f);
		const auto cameraDir = (cameraPos - cameraTarget).getNormalized();

		TEST(Matrix4D, LookAt)
		{
			auto rightUpDirMatrix = Matrix4D(Matrix4DArray
			{
				cameraRight.x(), cameraRight.y(), cameraRight.z(), 0.0f,
				cameraUp.x(), cameraUp.y(), cameraUp.z(), 0.0f,
				cameraDir.x(), cameraDir.y(), cameraDir.z(), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
			}.data());

			auto posMatrix = Matrix4D(Matrix4DArray
			{
				1.0f, 0.0f, 0.0f, -cameraPos.x(),
				0.0f, 1.0f, 0.0f, -cameraPos.y(),
				0.0f, 0.0f, 1.0f, -cameraPos.z(),
				0.0f, 0.0f, 0.0f, 1.0f,
			}.data());

			auto testedLookAtMat = (rightUpDirMatrix * posMatrix).getTransposed();
			auto lookAtMat = Matrix4D::lookAt(cameraPos, cameraTarget, cameraUp);
			EXPECT_TRUE(testedLookAtMat == lookAtMat);
		}
	}
}