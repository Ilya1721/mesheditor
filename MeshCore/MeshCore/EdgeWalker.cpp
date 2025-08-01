#include "EdgeWalker.h"

#include "HalfEdge.h"

namespace MeshCore
{
  EdgeWalker::EdgeWalker(HalfEdge* startHalfEdge) : mStartHalfEdge(startHalfEdge) {}

  void EdgeWalker::forEach(const EdgeFunction& function)
  {
    auto currentHalfEdge = mStartHalfEdge;
    do {
      function(currentHalfEdge);
      currentHalfEdge = currentHalfEdge->next;
    } while (currentHalfEdge != mStartHalfEdge);
  }
}  // namespace MeshCore