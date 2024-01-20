#pragma once

namespace MeshCore
{
    struct HalfEdge;

    struct Face
    {
        HalfEdge* halfEdge = nullptr;
    };
}
