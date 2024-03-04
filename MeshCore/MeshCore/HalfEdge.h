#pragma once

namespace MeshCore
{
    struct UniqueVertex;
    struct Face;

    struct HalfEdge
    {
        HalfEdge* prev = nullptr;
        HalfEdge* next = nullptr;
        HalfEdge* twin = nullptr;

        UniqueVertex* vertex = nullptr;
        Face* face = nullptr;
    };
}
