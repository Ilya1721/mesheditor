#include "TestUtils.h"

namespace Operators
{
	TEST(Vector2D, EqualityOperator)
	{
		Vector2D vec(testX, testY);
		Vector2D equalVec(testX, testY);
		EXPECT_TRUE(vec == equalVec);
		Vector2D notEqualVec(testX + 1.0f, testY);
		EXPECT_FALSE(vec == notEqualVec);
		EXPECT_TRUE(vec != notEqualVec);
	}

	TEST(Vector2D, AtIndexOperator)
	{
		Vector2D vec(testX, testY);
		EXPECT_FLOAT_EQ(vec[0], testX);
		EXPECT_FLOAT_EQ(vec[1], testY);
	}

	TEST(Vector2D, DotProduct)
	{
		Vector2D vec(1.0f, 2.0f);
		Vector2D otherVec(2.0f, 3.0f);
		EXPECT_FLOAT_EQ(vec * otherVec, 8.0f);
	}

	TEST(Vector3D, DotProduct)
	{
		Vector3D vec(1.0f, 2.0f, 3.0f);
		Vector3D otherVec(2.0f, 3.0f, 4.0f);
		EXPECT_FLOAT_EQ(vec * otherVec, 20.0f);
	}

	TEST(Vector4D, DotProduct)
	{
		Vector4D vec(1.0f, 2.0f, 3.0f, 1.0f);
		Vector4D otherVec(2.0f, 3.0f, 4.0f, 2.0f);
		EXPECT_FLOAT_EQ(vec * otherVec, 22.0f);
	}

	TEST(Vector2D, VecPlusVec)
	{
		Vector2D vec(1.0f, 2.0f);
		Vector2D otherVec(2.0f, 3.0f);
		auto result = vec + otherVec;
		EXPECT_TRUE(result == (Vector2D{ 3.0f, 5.0f }));
	}

	TEST(Vector3D, VecPlusVec)
	{
		Vector3D vec(1.0f, 2.0f, 3.0f);
		Vector3D otherVec(2.0f, 3.0f, 4.0f);
		auto result = vec + otherVec;
		EXPECT_TRUE(result == (Vector3D{ 3.0f, 5.0f, 7.0f }));
	}

	TEST(Vector4D, VecPlusVec)
	{
		Vector4D vec(1.0f, 2.0f, 3.0f, 4.0f);
		Vector4D otherVec(2.0f, 3.0f, 4.0f, 5.0f);
		auto result = vec + otherVec;
		EXPECT_TRUE(result == (Vector4D{ 3.0f, 5.0f, 7.0f, 9.0f }));
	}

	TEST(Vector2D, VecMinusVec)
	{
		Vector2D vec(1.0f, 2.0f);
		Vector2D otherVec(2.0f, 3.0f);
		auto result = vec - otherVec;
		EXPECT_TRUE(result == (Vector2D{ -1.0f, -1.0f }));
	}

	TEST(Vector3D, VecMinusVec)
	{
		Vector3D vec(1.0f, 2.0f, 3.0f);
		Vector3D otherVec(2.0f, 3.0f, 4.0f);
		auto result = vec - otherVec;
		EXPECT_TRUE(result == (Vector3D{ -1.0f, -1.0f, -1.0f }));
	}

	TEST(Vector4D, VecMinusVec)
	{
		Vector4D vec(1.0f, 2.0f, 3.0f, 6.0f);
		Vector4D otherVec(2.0f, 3.0f, 4.0f, 5.0f);
		auto result = vec - otherVec;
		EXPECT_TRUE(result == (Vector4D{ -1.0f, -1.0f, -1.0f, 1.0f }));
	}

	TEST(Vector2D, VecDivideScalar)
	{
		Vector2D vec(1.0f, 2.0f);
		Vector2D otherVec(2.0f, 3.0f);
		auto result = vec / 2.0f;
		EXPECT_TRUE(result == (Vector2D{ 0.5f, 1.0f }));
	}

	TEST(Vector3D, VecDivideScalar)
	{
		Vector3D vec(1.0f, 2.0f, 3.0f);
		Vector3D otherVec(2.0f, 3.0f, 2.0f);
		auto result = vec / 2.0f;
		EXPECT_TRUE(result == (Vector3D{ 0.5f, 1.0f, 1.5f }));
	}

	TEST(Vector4D, VecDivideScalar)
	{
		Vector4D vec(1.0f, 2.0f, 3.0f, 4.0f);
		Vector4D otherVec(2.0f, 3.0f, 2.0f, 2.0f);
		auto result = vec / 2.0f;
		EXPECT_TRUE(result == (Vector4D{ 0.5f, 1.0f, 1.5f, 2.0f }));
	}

	TEST(Vector2D, VecPlusEqualVec)
	{
		Vector2D vec(1.0f, 2.0f);
		Vector2D otherVec(2.0f, 3.0f);
		vec += otherVec;
		EXPECT_TRUE(vec == (Vector2D{ 3.0f, 5.0f }));
	}

	TEST(Vector3D, VecPlusEqualVec)
	{
		Vector3D vec(1.0f, 2.0f, 3.0f);
		Vector3D otherVec(2.0f, 3.0f, 4.0f);
		vec += otherVec;
		EXPECT_TRUE(vec == (Vector3D{ 3.0f, 5.0f, 7.0f }));
	}

	TEST(Vector4D, VecPlusEqualVec)
	{
		Vector4D vec(1.0f, 2.0f, 3.0f, 4.0f);
		Vector4D otherVec(2.0f, 3.0f, 4.0f, 5.0f);
		vec += otherVec;
		EXPECT_TRUE(vec == (Vector4D{ 3.0f, 5.0f, 7.0f, 9.0f }));
	}

	TEST(Vector2D, VecMultScalar)
	{
		Vector2D vec(1.0f, 2.0f);
		auto result = vec * 2.0f;
		EXPECT_TRUE(result == (Vector2D{ 2.0f, 4.0f }));
	}

	TEST(Vector3D, VecMultScalar)
	{
		Vector3D vec(1.0f, 2.0f, 3.0f);
		auto result = vec * 2.0f;
		EXPECT_TRUE(result == (Vector3D{ 2.0f, 4.0f, 6.0f }));
	}

	TEST(Vector4D, VecMultScalar)
	{
		Vector4D vec(1.0f, 2.0f, 3.0f, 4.0f);
		auto result = vec * 2.0f;
		EXPECT_TRUE(result == (Vector4D{ 2.0f, 4.0f, 6.0f, 8.0f }));
	}

	TEST(Vector2D, ScalarMultVec)
	{
		Vector2D vec(1.0f, 2.0f);
		auto result = 2.0f * vec;
		EXPECT_TRUE(result == (Vector2D{ 2.0f, 4.0f }));
	}

	TEST(Vector3D, ScalarMultVec)
	{
		Vector3D vec(1.0f, 2.0f, 3.0f);
		auto result = 2.0f * vec;
		EXPECT_TRUE(result == (Vector3D{ 2.0f, 4.0f, 6.0f }));
	}

	TEST(Vector4D, ScalarMultVec)
	{
		Vector4D vec(1.0f, 2.0f, 3.0f, 4.0f);
		auto result = 2.0f * vec;
		EXPECT_TRUE(result == (Vector4D{ 2.0f, 4.0f, 6.f, 8.0f }));
	}

	TEST(Vector2D, VecMinus)
	{
		Vector2D vec(1.0f, 2.0f);
		auto result = -vec;
		EXPECT_TRUE(result == (Vector2D{ -1.0f, -2.0f }));
	}

	TEST(Vector3D, VecMinus)
	{
		Vector3D vec(1.0f, 2.0f, 3.0f);
		auto result = -vec;
		EXPECT_TRUE(result == (Vector3D{ -1.0f, -2.0f, -3.0f }));
	}

	TEST(Vector4D, VecMinus)
	{
		Vector4D vec(1.0f, 2.0f, 3.0f, 4.0f);
		auto result = -vec;
		EXPECT_TRUE(result == (Vector4D{ -1.0f, -2.0f, -3.0f, -4.0f }));
	}
}