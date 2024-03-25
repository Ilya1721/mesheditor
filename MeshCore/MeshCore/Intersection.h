#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Surface.h"

namespace MeshCore
{
    struct RaySurfaceIntersection
    {
        Surface intersectedSurface;
        std::vector<int> surfaceIndices;
        glm::vec3 point {};

        void setClosest(const RaySurfaceIntersection& other, const glm::vec3& referencePoint);
    };
}
