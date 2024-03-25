#pragma once

#include <array>
#include <glm/glm.hpp>

namespace GeometryCore
{
    bool isCloser(const glm::vec3& left, const glm::vec3& right, const glm::vec3& reference);
    float getTriangleSquare(const std::array<float, 3>& edgesLengths);
}