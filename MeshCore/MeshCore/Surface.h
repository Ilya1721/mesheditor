#pragma once

#include <unordered_set>

#include <glm/glm.hpp>

namespace MeshCore
{
    struct Face;

    struct Surface
    {
        Surface() = default;
        Surface(Face* initialFace, bool collectAdjacentFaces = true);

        std::unordered_set<Face*> faces;
        glm::vec3 normal{};
    };
}
