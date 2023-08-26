#include "TestUtils.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "GeometryCore/Logger.h"

glm::mat4 getMat4(float scaleFactor)
{
	return glm::mat4(scaleFactor);
}

void logMatrixComparison(const glm::mat4& result, const glm::mat4& expected)
{
	std::cout << "Result = \n" << GeometryCore::getMatrixPrettyString(result) << std::endl;
	std::cout << "Expected = \n" << GeometryCore::getMatrixPrettyString(expected) << std::endl;
}

void logVectorComparison(const glm::vec2& first, const glm::vec2& second)
{
	return logVectorComparison(glm::vec3(first, 0.0f), glm::vec3(second, 0.0f));
}

void logVectorComparison(const glm::vec3& first, const glm::vec3& second)
{
	return logVectorComparison(glm::vec4(first, 0.0f), glm::vec4(second, 0.0f));
}

void logVectorComparison(const glm::vec4& result, const glm::vec4& expected)
{
	std::cout << "Result = \n" << GeometryCore::getVectorPrettyString(result) << std::endl;
	std::cout << "Expected = \n" << GeometryCore::getVectorPrettyString(expected) << std::endl;
}

float cotangent(float angleRad)
{
	return 1.0f / std::tanf(angleRad);
}

bool matrixEqualMatArray(const glm::mat4& matrix, const Matrix4DArray& matArray)
{
	auto matrixPtr = glm::value_ptr(matrix);
	for (glm::size_t matIdx = 0; matIdx < 16; ++matIdx)
	{
		if (glm::epsilonNotEqual(matrixPtr[matIdx], matArray[matIdx], EPSILON))
			return false;
	}
	return true;
}

glm::mat4 matFromMatArray(const Matrix4DArray& matArray)
{
	return glm::mat4(matArray[0], matArray[1], matArray[2], matArray[3],
					 matArray[4], matArray[5], matArray[6], matArray[7],
					 matArray[8], matArray[9], matArray[10], matArray[11],
					 matArray[12], matArray[13], matArray[14], matArray[15]);
}
