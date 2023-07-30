#include "TestUtils.h"

#include <iostream>


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

void logMatrixComparison(const Matrix4D& result, const Matrix4D& expected)
{
	std::cout << "Result = \n" << result.getPrettyString() << std::endl;
	std::cout << "Expected = \n" << expected.getPrettyString() << std::endl;
}

void logVectorComparison(const Vector2D& result, const Vector2D& expected)
{
	std::cout << "Result = \n" << result.getPrettyString() << std::endl;
	std::cout << "Expected = \n" << expected.getPrettyString() << std::endl;
}

float cotangent(float angleRad)
{
	return 1.0f / std::tanf(angleRad);
}
