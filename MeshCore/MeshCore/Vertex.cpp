#include "Vertex.h"

#include "GeometryCore/Numeric.h"
#include "GeometryCore/Transforms.h"

namespace MeshCore
{
  Vertex::Vertex(const Point3D& otherPos, const Vector3D& otherNormal)
    : pos(otherPos), normal(otherNormal)
  {
  }

  bool Vertex::operator==(const Vertex& other) const { return isEqual(pos, other.pos); }

  Vertex operator*(const glm::mat4& transform, const Vertex& vertex)
  {
    Vertex newVertex {};
    newVertex.pos = transformPoint(vertex.pos, transform);
    newVertex.normal = glm::normalize(transformVector(vertex.normal, transform));

    return newVertex;
  }

  UniqueVertex::UniqueVertex(Vertex& originalVertex, int originalVertexIndex)
    : Vertex(originalVertex)
  {
    originalVertices.emplace_back(&originalVertex, originalVertexIndex);
    adjacentFacesNormals.insert(originalVertex.normal);
  }

  void UniqueVertex::updatePos(const Point3D& otherPos)
  {
    pos = otherPos;
    for (auto& originalVertexData : originalVertices)
    {
      originalVertexData.vertex->pos = pos;
    }
  }

  void UniqueVertex::updateNormal(const Vector3D& otherNormal) { normal = otherNormal; }
}  // namespace MeshCore