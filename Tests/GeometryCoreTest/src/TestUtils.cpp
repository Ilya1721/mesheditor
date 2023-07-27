#include "TestUtils.h"

void EXPECT_VEC_EQ(const Vector4D& firstVec, const Vector4D& secondVec)
{
	EXPECT_NEAR(firstVec.x(), firstVec.x(), EPSILON);
	EXPECT_NEAR(firstVec.y(), firstVec.y(), EPSILON);
	EXPECT_NEAR(firstVec.z(), firstVec.z(), EPSILON);
	EXPECT_NEAR(firstVec.w(), firstVec.w(), EPSILON);
}

void EXPECT_MAT_EQ(const Matrix4D& firstMat, const Matrix4DArray& secondMatArray)
{
	const auto& firstMatPtr = firstMat.valuePtr();
	for (int matIndex = 0; matIndex < secondMatArray.size(); ++matIndex)
	{
		auto first = firstMatPtr[matIndex];
		auto second = secondMatArray[matIndex];
		EXPECT_NEAR(firstMatPtr[matIndex], secondMatArray[matIndex], EPSILON);
	}
}

Vector2D getVec2()
{
	return Vector2D(testX, testY);
}

Vector3D getVec3()
{
	return Vector3D(testX, testY, testZ);
}

Vector4D getVec4()
{
	return Vector4D(testX, testY, testZ, testW);
}

Matrix4D getMat4(float scaleFactor)
{
	return Matrix4D(scaleFactor);
}

float cotangent(float angleRad)
{
	return 1.0f / std::tanf(angleRad);
}
