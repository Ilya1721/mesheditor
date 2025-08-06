#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <unordered_set>
#include <vector>

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
    Face(Mesh* parentMesh, HalfEdge* halfEdge = nullptr);

    Mesh* parentMesh = nullptr;
    HalfEdge* halfEdge = nullptr;

    std::optional<Point3D> getIntersectionPoint(const Ray& ray) const;
    Vector3D calcNormal() const;
    bool isPointInside(const Point3D& point) const;
    std::vector<Vector3D> getAllEdges() const;
    std::vector<HalfEdge*> getAllHalfEdges() const;
    std::unordered_set<Face*> getAdjacentFaces(
      bool filterByNormal = false, const Vector3D* normalPtr = nullptr
    ) const;
    HalfEdge* findOutgoingEdge(const UniqueVertex* vertex) const;
    float getSquare() const;
    std::vector<Point3D> getVerticesPositions() const;

    void move(
      const Vector3D& movement, std::unordered_set<UniqueVertex*>& alreadyChangedVertices
    );
  };
}  // namespace MeshCore
