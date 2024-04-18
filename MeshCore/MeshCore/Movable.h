#pragma once

#include <unordered_set>

#include "GeometryCore/Typedefs.h"

namespace MeshCore
{
    struct UniqueVertex;

    struct Movable
    {
        virtual ~Movable() = default;
        virtual void move(const GeometryCore::Vector3D& movement, std::unordered_set<UniqueVertex*>& alreadyChangedVertices) = 0;
    };
}