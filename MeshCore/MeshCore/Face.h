#pragma once

#include <vector>
#include <unordered_set>

#include "GeometryCore/Ray.h"

namespace MeshCore
{
    struct HalfEdge;
    struct Vertex;

    struct Face
    {
        HalfEdge* halfEdge = nullptr;

        std::optional<glm::vec3> getIntersectionPoint(const GeometryCore::Ray& ray) const;
        glm::vec3 calcNormal() const;
        bool isPointInside(const glm::vec3& point, const glm::vec3& faceNormal) const;
        std::vector<glm::vec3> getAllGeometryEdges() const;
        std::vector<HalfEdge*> getAllEdges() const;
        std::unordered_set<Face*> getAdjacentFaces(bool filterByNormal = false, const glm::vec3* normalPtr = nullptr) const;
        HalfEdge* findOutgoingEdge(const Vertex* vertex) const;

    private:
        std::vector<glm::vec3> getAllVerticesToPointVectors(const glm::vec3& point) const;
    };
}
