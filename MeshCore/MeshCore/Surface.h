#pragma once

#include <unordered_set>

#include <glm/glm.hpp>

#include "GeometryCore/Plane.h"

namespace MeshCore
{
    struct Face;

    struct Surface
    {
        Surface() = default;
        Surface(Face* initialFace, bool collectAdjacentFaces = true);

        GeometryCore::Plane getPerpendicularPlane() const;

        std::unordered_set<Face*> faces;
        glm::vec3 normal{};

    private:
        Face* mInitialFace = nullptr;
    };
}
