#pragma once

#include <array>
#include <glm/glm.hpp>

#include "Typedefs.h"

namespace GeometryCore
{
    bool isCloser(const Point3D& left, const Point3D& right, const Point3D& reference);
    float getTriangleSquare(const std::array<float, 3>& edgesLengths);
}