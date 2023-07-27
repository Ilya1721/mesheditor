#include "TestUtils.h"

namespace Operators
{
	TEST(Matrix4D, EqualityOperator)
	{
		Matrix4D mat(scaleFactor);
		Matrix4D equalMat(mat);
		EXPECT_TRUE(mat == equalMat);
		Matrix4D notEqualMat(1.0f);
		EXPECT_FALSE(mat == notEqualMat);
		EXPECT_TRUE(mat != notEqualMat);
	}

	TEST(Matrix4D, MatMultMat)
	{
		Matrix4D firstMatrix(scaleFactor);
		Matrix4D secondMatrix(scaleFactor);
		auto result = firstMatrix * secondMatrix;
		EXPECT_MAT_EQ(result, Matrix4DArray{ 4.0f, 0.0f, 0.0f, 0.0f,
											 0.0f, 4.0f, 0.0f, 0.0f,
											 0.0f, 0.0f, 4.0f, 0.0f,
											 0.0f, 0.0f, 0.0f, 4.0f });
	}

	TEST(Matrix4D, MatMultVec)
	{
		Matrix4D scaleMatrix(scaleFactor);
		Vector4D vec4(1.0f, 1.0f, 1.0f, 1.0f);
		auto result = scaleMatrix * vec4;
		EXPECT_VEC_EQ(result, Vector4D{ 2.0f, 2.0f, 2.0f, 2.0f });
	}
}