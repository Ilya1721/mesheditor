#pragma once

#include <glm/glm.hpp>

namespace GeometryCore
{
    bool isEqual(double left, double right);
    bool isCloser(const glm::vec3& left, const glm::vec3& right, const glm::vec3& reference);
}