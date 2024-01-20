#pragma once

namespace MeshCore
{
    struct Vertex;
    struct Face;

    struct HalfEdge
    {
        HalfEdge* prev = nullptr;
        HalfEdge* next = nullptr;
        HalfEdge* twin = nullptr;

        Vertex* vertex = nullptr;
        Face* face = nullptr;
    };
}
