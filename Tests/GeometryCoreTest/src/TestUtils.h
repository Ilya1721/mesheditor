#pragma once

#include <array>
#include <gtest/gtest.h>
#include <glm/glm.hpp>

#include "GeometryCore/Constants.h"

using namespace GeometryCore;

constexpr float testX = 5.0f;
constexpr float testY = 7.0f;
constexpr float testZ = 9.0f;
constexpr float testW = 11.0f;
constexpr float scaleFactor = 2.0f;

using Matrix4DArray = std::array<float, 16>;

const auto identityMatrix = Matrix4DArray
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

const auto scaleMatrix = Matrix4DArray
{ 
	scaleFactor, 0.0f, 0.0f, 0.0f,
	0.0f, scaleFactor, 0.0f, 0.0f,
	0.0f, 0.0f, scaleFactor, 0.0f,
	0.0f, 0.0f, 0.0f, scaleFactor 
};

glm::mat4 getMat4(float scaleFactor = 1.0f);

void logMatrixComparison(const glm::mat4& first, const glm::mat4& second);
void logVectorComparison(const glm::vec2& first, const glm::vec2& second);
void logVectorComparison(const glm::vec3& first, const glm::vec3& second);
void logVectorComparison(const glm::vec4& first, const glm::vec4& second);
float cotangent(float angleRad);

bool matrixEqualMatArray(const glm::mat4& matrix, const Matrix4DArray& matArray);
glm::mat4 matFromMatArray(const Matrix4DArray& matArray);
