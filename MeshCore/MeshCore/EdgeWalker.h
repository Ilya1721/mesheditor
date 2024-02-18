#pragma once

#include <functional>

namespace MeshCore
{
    struct HalfEdge;

    using EdgeFunction = std::function<void(HalfEdge*)>;

    class EdgeWalker
    {
    public:
        EdgeWalker(HalfEdge* startEdge);

        void forEach(const EdgeFunction& edgeFunction);

    private:
        HalfEdge* mStartHalfEdge;
    };
}