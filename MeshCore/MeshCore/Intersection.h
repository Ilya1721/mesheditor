#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"
#include "Surface.h"

namespace MeshCore
{
    using namespace GeometryCore;

    struct RaySurfaceIntersection
    {
        Surface intersectedSurface;
        std::vector<int> surfaceIndices;
        Point3D point {};

        void setClosest(const RaySurfaceIntersection& other, const Point3D& referencePoint);
    };
}