#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <unordered_set>
#include <vector>

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

    std::optional<glm::vec3> getIntersectionPoint(const Ray& ray) const;
    glm::vec3 calcNormal() const;
    bool isPointInside(const glm::vec3& point) const;
    std::vector<glm::vec3> getAllEdges() const;
    std::vector<HalfEdge*> getAllHalfEdges() const;
    std::unordered_set<Face*> getAdjacentFaces(
      bool filterByNormal = false, const glm::vec3* normalPtr = nullptr
    ) const;
    HalfEdge* findOutgoingEdge(const UniqueVertex* vertex) const;
    float getSquare() const;
    std::vector<glm::vec3> getVerticesPositions() const;

    void move(
      const glm::vec3& movement, std::unordered_set<UniqueVertex*>& alreadyChangedVertices
    );
  };
}  // namespace MeshCore
