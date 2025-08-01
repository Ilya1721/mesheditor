#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_set>

#include "GeometryCore/Typedefs.h"

namespace MeshCore
{
  using namespace GeometryCore;

  struct HalfEdge;

  struct Vertex
  {
    Vertex() = default;
    Vertex(const Point3D& otherPos, const Vector3D& otherNormal);

    Point3D pos {};
    Vector3D normal {};
    HalfEdge* halfEdge = nullptr;

    bool operator==(const Vertex& other) const;
    friend Vertex operator*(const glm::mat4& transform, const Vertex& vertex);
  };

  struct OriginalVertexData
  {
    Vertex* vertex;
    int index;
  };

  struct UniqueVertex : Vertex
  {
    UniqueVertex(Vertex& originalVertex, int originalVertexIndex);

    void updatePos(const Point3D& otherPos);
    void updateNormal(const Vector3D& otherNormal);

    std::vector<OriginalVertexData> originalVertices;
    std::unordered_set<Vector3D> adjacentFacesNormals;
  };

  using HalfEdgeVerticesPair = std::pair<Vertex, Vertex>;
}  // namespace MeshCore
