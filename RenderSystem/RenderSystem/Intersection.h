#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"
#include "MeshCore/Surface.h"
#include "MeshCore/Intersection.h"

namespace RenderSystem
{
    using namespace GeometryCore;

    struct RaySurfaceIntersection
    {
        MeshCore::Surface surface {};
        std::vector<int> surfaceIndices;
        Point3D point {};

        void setClosest(const MeshCore::MeshIntersectionData& meshIntersectionData, const Point3D& referencePoint);
    };
}