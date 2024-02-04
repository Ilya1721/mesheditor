#pragma once

#include <glm/glm.hpp>

#include "Face.h"

namespace MeshCore
{
    struct RayFaceIntersection
    {
        Face* face = nullptr;
        glm::vec3 point {};

        void setClosest(const RayFaceIntersection& other, const glm::vec3& referencePoint);
    };
}
