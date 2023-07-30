#include "TestUtils.h"

namespace Constructors
{
	TEST(Vector2D, DefaultConstructor)
	{
		Vector2D vec;
		EXPECT_TRUE(vec == (Vector2D{ 0.0f, 0.0f }));
	}

	TEST(Vector3D, DefaultConstructor)
	{
		Vector3D vec;
		EXPECT_TRUE(vec == (Vector3D{ 0.0f, 0.0f, 0.0f }));
	}

	TEST(Vector4D, DefaultConstructor)
	{
		Vector4D vec;
		EXPECT_TRUE(vec == (Vector4D{ 0.0f, 0.0f, 0.0f, 0.0f }));
	}

	TEST(Vector2D, XYConstructor)
	{
		Vector2D vec(testX, testY);
		EXPECT_TRUE(vec == (Vector2D{ testX, testY }));
	}

	TEST(Vector3D, XYZConstructor)
	{
		Vector3D vec(testX, testY, testZ);
		EXPECT_TRUE(vec == (Vector3D{ testX, testY, testZ }));
	}

	TEST(Vector4D, XYZWConstructor)
	{
		Vector4D vec(testX, testY, testZ, testW);
		EXPECT_TRUE(vec == (Vector4D{ testX, testY, testZ, testW }));
	}

	TEST(Vector2D, CopyConstructor)
	{
		Vector2D orig(testX, testY);
		Vector2D copy(orig);
		EXPECT_TRUE(copy == (Vector2D{ testX, testY }));
	}

	TEST(Vector2D, CopyFromVec3Constructor)
	{
		Vector3D orig(testX, testY, 1.0f);
		Vector2D copy(orig);
		EXPECT_TRUE(copy == (Vector2D{ testX, testY }));
		EXPECT_FLOAT_EQ(copy[2], 0.0f);
	}

	TEST(Vector3D, CopyConstructor)
	{
		Vector3D orig(testX, testY, testZ);
		Vector3D copy(orig);
		EXPECT_TRUE(copy == (Vector3D{ testX, testY, testZ }));
	}

	TEST(Vector3D, CopyFromVec4Constructor)
	{
		Vector4D orig(testX, testY, testZ, 1.0f);
		Vector3D copy(orig);
		EXPECT_TRUE(copy == (Vector3D{ testX, testY, testZ }));
		EXPECT_FLOAT_EQ(copy[3], 0.0f);
	}

	TEST(Vector4D, CopyConstructor)
	{
		Vector4D orig(testX, testY, testZ, testW);
		Vector4D copy(orig);
		EXPECT_TRUE(copy == (Vector4D{ testX, testY, testZ, testW }));
	}

	TEST(Vector2D, CopyFromVec3Assignment)
	{
		Vector3D orig(testX, testY, 1.0f);
		Vector2D copy;
		copy = orig;
		EXPECT_TRUE(copy == (Vector2D{ testX, testY }));
		EXPECT_FLOAT_EQ(copy[2], 0.0f);
	}

	TEST(Vector3D, CopyAssignment)
	{
		Vector3D orig(testX, testY, testZ);
		Vector3D copy;
		copy = orig;
		EXPECT_TRUE(copy == (Vector3D{ testX, testY, testZ }));
	}

	TEST(Vector3D, CopyFromVec4Assignment)
	{
		Vector4D orig(testX, testY, testZ, 1.0f);
		Vector3D copy;
		copy = orig;
		EXPECT_TRUE(copy == (Vector3D{ testX, testY, testZ }));
		EXPECT_FLOAT_EQ(copy[3], 0.0f);
	}

	TEST(Vector4D, CopyAssignment)
	{
		Vector4D orig(testX, testY, testZ, testW);
		Vector4D copy;
		copy = orig;
		EXPECT_TRUE(copy == (Vector4D{ testX, testY, testZ, testW }));
	}

	TEST(Vector2D, MoveConstructor)
	{
		Vector2D copy(getVec2());
		EXPECT_TRUE(copy == (Vector2D{ testX, testY }));
	}

	TEST(Vector2D, MoveFromVec3Constructor)
	{
		Vector2D copy(getVec3());
		EXPECT_TRUE(copy == (Vector2D{ testX, testY }));
		EXPECT_FLOAT_EQ(copy[2], 0.0f);
	}

	TEST(Vector3D, MoveFromVec4Constructor)
	{
		Vector3D copy(getVec4());
		EXPECT_TRUE(copy == (Vector3D{ testX, testY, testZ }));
		EXPECT_FLOAT_EQ(copy[3], 0.0f);
	}

	TEST(Vector4D, MoveConstructor)
	{
		Vector4D copy(getVec4());
		EXPECT_TRUE(copy == (Vector4D{ testX, testY, testZ, testW }));
	}

	TEST(Vector2D, MoveAssignment)
	{
		Vector2D copy;
		copy = std::move(getVec2());
		EXPECT_TRUE(copy == (Vector2D{ testX, testY }));
	}

	TEST(Vector2D, MoveFromVec3Assignment)
	{
		Vector2D copy;
		copy = std::move(getVec3());
		EXPECT_TRUE(copy == (Vector2D{ testX, testY }));
		EXPECT_FLOAT_EQ(copy[2], 0.0f);
	}

	TEST(Vector3D, MoveAssignment)
	{
		Vector3D copy;
		copy = std::move(getVec3());
		EXPECT_TRUE(copy == (Vector3D{ testX, testY, testZ }));
	}

	TEST(Vector3D, MoveFromVec4Assignment)
	{
		Vector3D copy;
		copy = std::move(getVec4());
		EXPECT_TRUE(copy == (Vector3D{ testX, testY, testZ }));
		EXPECT_FLOAT_EQ(copy[3], 0.0f);
	}

	TEST(Vector4D, MoveAssignment)
	{
		Vector4D copy;
		copy = std::move(getVec4());
		EXPECT_TRUE(copy == (Vector4D{ testX, testY, testZ, testW }));
	}

	TEST(Vector4D, Vec3CopyConstructor)
	{
		Vector3D orig(testX, testY, testZ);
		Vector4D copy(orig, testW);
		EXPECT_TRUE(copy == (Vector4D{ testX, testY, testZ, testW }));
	}

	TEST(Vector4D, Vec3MoveConstructor)
	{
		Vector4D copy(getVec3());
		EXPECT_TRUE(copy == (Vector4D{ testX, testY, testZ, 0.0f }));
	}
}