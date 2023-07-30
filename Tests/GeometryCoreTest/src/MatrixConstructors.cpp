#include "TestUtils.h"

namespace Constructors
{
	TEST(Matrix4D, DefaultConstructor)
	{
		Matrix4D matrix;
		EXPECT_TRUE(matrix == identityMatrix);
	}

	TEST(Matrix4D, OneValueConstructor)
	{
		Matrix4D matrix(scaleFactor);
		EXPECT_TRUE(matrix == scaleMatrix);
	}

	TEST(Matrix4D, MatArray)
	{
		auto matArray = Matrix4DArray{ 1.0f, 2.0f, 3.0f, 4.0f,
								  5.0f, 6.0f, 7.0f, 8.0f,
								  9.0f, 10.0f, 11.0f, 12.0f,
								  13.0f, 14.0f, 15.0f, 16.0f };

		Matrix4D testMat(matArray.data());
		auto testMatValuePtr = testMat.valuePtr();
		for (size_t matIdx = 0; matIdx < VALUES_IN_MATRIX4D; ++matIdx)
		{
			EXPECT_FLOAT_EQ(testMatValuePtr[matIdx], matArray[matIdx]);
		}
	}

	TEST(Matrix4D, CopyConstructor)
	{
		Matrix4D matrix(1.0f);
		Matrix4D copy(matrix);
		EXPECT_TRUE(copy == identityMatrix);
	}

	TEST(Matrix4D, MoveConstructor)
	{
		Matrix4D matrix(1.0f);
		Matrix4D copy(getMat4());
		EXPECT_TRUE(copy == identityMatrix);
	}

	TEST(Matrix4D, CopyAssignment)
	{
		Matrix4D orig(scaleFactor);
		Matrix4D copy;
		copy = orig;
		EXPECT_TRUE(copy == scaleMatrix);
	}

	TEST(Matrix4D, MoveAssignment)
	{
		Matrix4D orig(1.0f);
		Matrix4D copy;
		copy = getMat4(scaleFactor);
		EXPECT_TRUE(copy == scaleMatrix);
	}
}