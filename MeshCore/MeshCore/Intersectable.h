#pragma once

#include "Intersection.h"

namespace MeshCore
{
    struct Intersectable
    {
        virtual ~Intersectable() = default;
        virtual RaySurfaceIntersection findIntersection(const GeometryCore::Ray& ray, bool intersectSurface, int passedFacesCount = 0) const = 0;
    };
}
