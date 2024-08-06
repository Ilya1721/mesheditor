#pragma once

#include <unordered_set>
#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"

namespace GeometryCore
{
    struct Plane;
}

namespace MeshCore
{
    using namespace GeometryCore;

    struct Face;
    class Object3D;

    struct Surface
    {
        Surface() = default;
        Surface(Face* initialFace, bool collectAdjacentFaces = true);

        Plane getPerpendicularPlane() const;
        Object3D* getParentObject() const;

        std::unordered_set<Face*> faces;
        Vector3D normal{};

    private:
        Face* mInitialFace = nullptr;
    };
}
