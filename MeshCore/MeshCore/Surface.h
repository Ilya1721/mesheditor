#pragma once

#include <unordered_set>

#include <glm/glm.hpp>

namespace MeshCore
{
    struct Face;

    struct Surface
    {
        Surface(Face* initialFace);

        std::unordered_set<Face*> faces;
        glm::vec3 normal{};
    };
}
