#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace MeshCore
{
    struct RayFaceIntersection
    {
        int faceIdx = -1;
        glm::vec3 point {};

        void setClosest(const RayFaceIntersection& other, const glm::vec3& referencePoint);
    };

    struct RaySurfaceIntersection : RayFaceIntersection
    {
        std::vector<int> surfaceFacesIndices;
    };
}
