#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <optional>
#include <unordered_set>

#include "GeometryCore/Typedefs.h"

namespace GeometryCore
{
    struct Ray;
}

namespace MeshCore
{
    using namespace GeometryCore;

    struct HalfEdge;
    struct UniqueVertex;
    class Mesh;

    struct Face
    {
        Mesh* parentMesh = nullptr;
        HalfEdge* halfEdge = nullptr;

        std::optional<Point3D> getIntersectionPoint(const Ray& ray) const;
        Vector3D calcNormal() const;
        bool isPointInside(const Point3D& point) const;
        std::vector<Vector3D> getAllGeometryEdges() const;
        std::vector<HalfEdge*> getAllEdges() const;
        std::unordered_set<Face*> getAdjacentFaces(bool filterByNormal = false, const Vector3D* normalPtr = nullptr) const;
        HalfEdge* findOutgoingEdge(const UniqueVertex* vertex) const;
        float getSquare() const;
        std::vector<Point3D> getVerticesPositions() const;

        void move(const Vector3D& movement, std::unordered_set<UniqueVertex*>& alreadyChangedVertices);
    };
}
