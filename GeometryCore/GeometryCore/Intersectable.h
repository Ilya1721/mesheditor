#pragma once

#include <glm/glm.hpp>
#include <optional>

#include "Typedefs.h"

namespace GeometryCore
{
    struct Ray;

    struct Intersectable
    {
        virtual ~Intersectable() = default;
        virtual std::optional<Point3D> findIntersectionPoint(const Ray& ray) const = 0;
    };
}