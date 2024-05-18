#pragma once

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "Typedefs.h"

namespace GeometryCore
{
    bool isCloser(const Point3D& left, const Point3D& right, const Point3D& reference);
    float getTriangleSquare(const std::array<float, 3>& edgesLengths);

    template <typename Vector>
    bool isEqual(const Vector& first, const Vector& second)
    {
        return glm::all(glm::epsilonEqual(first, second, 1e-6f));
    }
}