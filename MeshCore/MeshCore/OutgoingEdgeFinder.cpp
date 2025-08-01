#include "OutgoingEdgeFinder.h"

#include "EdgeWalker.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Vertex.h"

namespace MeshCore
{
  OutgoingEdgeFinder::OutgoingEdgeFinder(HalfEdge* startOutgoingEdge)
    : mStartOutgoingEdge(startOutgoingEdge), mVertex(startOutgoingEdge->vertex)
  {
  }

  void OutgoingEdgeFinder::collectAll() { collectNextOutgoing(mStartOutgoingEdge); }

  void OutgoingEdgeFinder::collectNextOutgoing(HalfEdge* currentOutgoing)
  {
    EdgeWalker edgeWalker(currentOutgoing->twin);
    edgeWalker.forEach(
      [this](HalfEdge* edge)
      {
        if (edge != mStartOutgoingEdge && isOutgoing(edge))
        {
          addOutgoing(edge);
          collectNextOutgoing(edge);
        }
      }
    );
  }

  bool OutgoingEdgeFinder::isOutgoing(HalfEdge* halfEdge) const
  {
    return halfEdge->vertex == mVertex;
  }

  void OutgoingEdgeFinder::addOutgoing(HalfEdge* halfEdge)
  {
    mOutgoingEdges.push_back(halfEdge);
    mOutgoingEdgesFaces.push_back(halfEdge->face);
  }

  const std::vector<Face*>& OutgoingEdgeFinder::getOutgoingEdgesFaces() const
  {
    return mOutgoingEdgesFaces;
  }

  const std::vector<HalfEdge*>& OutgoingEdgeFinder::getOutgoingEdges() const
  {
    return mOutgoingEdges;
  }
}  // namespace MeshCore