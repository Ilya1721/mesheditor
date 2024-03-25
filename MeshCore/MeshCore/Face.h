#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <optional>
#include <unordered_set>

namespace GeometryCore
{
    struct Ray;
}

namespace MeshCore
{
    struct HalfEdge;
    struct UniqueVertex;
    class Mesh;

    struct Face
    {
        Mesh* parentMesh = nullptr;
        HalfEdge* halfEdge = nullptr;

        std::optional<glm::vec3> getIntersectionPoint(const GeometryCore::Ray& ray) const;
        glm::vec3 calcNormal() const;
        bool isPointInside(const glm::vec3& point) const;
        std::vector<glm::vec3> getAllGeometryEdges() const;
        std::vector<HalfEdge*> getAllEdges() const;
        std::unordered_set<Face*> getAdjacentFaces(bool filterByNormal = false, const glm::vec3* normalPtr = nullptr) const;
        HalfEdge* findOutgoingEdge(const UniqueVertex* vertex) const;
        float getSquare() const;
        std::vector<glm::vec3> getVerticesPositions() const;

        void move(const glm::vec3& movement, std::unordered_set<UniqueVertex*>& alreadyChangedVertices);
    };
}
