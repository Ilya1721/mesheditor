#pragma once

#include <array>

#include "gtest/gtest.h"

#include "GeometryCore/Vector.h"
#include "GeometryCore/Matrix.h"
#include "GeometryCore/Constants.h"

using namespace Geometry;

constexpr float testX = 5.0f;
constexpr float testY = 7.0f;
constexpr float testZ = 9.0f;
constexpr float testW = 11.0f;
constexpr float scaleFactor = 2.0f;

using Matrix4DArray = std::array<float, VALUES_IN_MATRIX4D>;

const auto identityMatrix = Matrix4DArray{ 1.0f, 0.0f, 0.0f, 0.0f,
										   0.0f, 1.0f, 0.0f, 0.0f,
										   0.0f, 0.0f, 1.0f, 0.0f,
										   0.0f, 0.0f, 0.0f, 1.0f };

const auto scaleMatrix = Matrix4DArray{ scaleFactor, 0.0f, 0.0f, 0.0f,
										0.0f, scaleFactor, 0.0f, 0.0f,
										0.0f, 0.0f, scaleFactor, 0.0f,
										0.0f, 0.0f, 0.0f, scaleFactor };

void EXPECT_VEC_EQ(const Vector4D& firstVec, const Vector4D& secondVec);
void EXPECT_MAT_EQ(const Matrix4D& firstMat, const Matrix4DArray& secondMatArray);

Vector2D getVec2();
Vector3D getVec3();
Vector4D getVec4();
Matrix4D getMat4(float scaleFactor = 1.0f);

float cotangent(float angleRad);
