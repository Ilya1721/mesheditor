#include "EdgeWalker.h"

#include "HalfEdge.h"

namespace MeshCore
{
    EdgeWalker::EdgeWalker(HalfEdge* startHalfEdge) :
        mStartHalfEdge(startHalfEdge)
    {}

    void EdgeWalker::forEach(const EdgeFunction& edgeFunction)
    {
        auto currentHalfEdge = mStartHalfEdge;
        do
        {
            edgeFunction(currentHalfEdge);
            currentHalfEdge = currentHalfEdge->next;
        } while (currentHalfEdge != mStartHalfEdge);
    }
}