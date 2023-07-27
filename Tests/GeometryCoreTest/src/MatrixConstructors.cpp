#include "TestUtils.h"

namespace Constructors
{
	TEST(Matrix4D, DefaultConstructor)
	{
		Matrix4D matrix;
		EXPECT_MAT_EQ(matrix, identityMatrix);
	}

	TEST(Matrix4D, OneValueConstructor)
	{
		Matrix4D matrix(scaleFactor);
		EXPECT_MAT_EQ(matrix, scaleMatrix);
	}

	TEST(Matrix4D, MatArray)
	{
		auto mat = Matrix4DArray{ 1.0f, 2.0f, 3.0f, 4.0f,
								  5.0f, 6.0f, 7.0f, 8.0f,
								  9.0f, 10.0f, 11.0f, 12.0f,
								  13.0f, 14.0f, 15.0f, 16.0f };

		Matrix4D testMat(mat.data());
		EXPECT_MAT_EQ(testMat, mat);
	}

	TEST(Matrix4D, CopyConstructor)
	{
		Matrix4D matrix(1.0f);
		Matrix4D copy(matrix);
		EXPECT_MAT_EQ(copy, identityMatrix);
	}

	TEST(Matrix4D, MoveConstructor)
	{
		Matrix4D matrix(1.0f);
		Matrix4D copy(getMat4());
		EXPECT_MAT_EQ(copy, identityMatrix);
	}

	TEST(Matrix4D, CopyAssignment)
	{
		Matrix4D orig(scaleFactor);
		Matrix4D copy;
		copy = orig;
		EXPECT_MAT_EQ(copy, scaleMatrix);
	}

	TEST(Matrix4D, MoveAssignment)
	{
		Matrix4D orig(1.0f);
		Matrix4D copy;
		copy = getMat4(scaleFactor);
		EXPECT_MAT_EQ(copy, scaleMatrix);
	}
}