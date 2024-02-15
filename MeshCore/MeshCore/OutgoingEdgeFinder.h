#pragma once

#include <vector>

namespace MeshCore
{
    struct HalfEdge;
    struct Vertex;
    struct Face;

    class OutgoingEdgeFinder
    {
    public:
        OutgoingEdgeFinder(HalfEdge* startOutgoingEdge);

        void collectAll();
        const std::vector<Face*>& getOutgoingEdgesFaces() const;
        const std::vector<HalfEdge*>& getOutgoingEdges() const;

    private:
        void collectNextOutgoing(HalfEdge* currentOutgoing);
        bool isOutgoing(HalfEdge* halfEdge) const;
        void addOutgoing(HalfEdge* halfEdge);

    private:
        HalfEdge* mStartOutgoingEdge;
        Vertex* mVertex;
        std::vector<Face*> mOutgoingEdgesFaces;
        std::vector<HalfEdge*> mOutgoingEdges;
    };
}