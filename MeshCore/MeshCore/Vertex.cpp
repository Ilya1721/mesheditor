#include "Vertex.h"

#include "GeometryCore/Numeric.h"
#include "GeometryCore/Transforms.h"

using namespace GeometryCore;

namespace MeshCore
{
  Vertex::Vertex(
    const glm::vec3& otherPos, const glm::vec3& otherNormal, const glm::vec2& otherTexture
  )
    : pos(otherPos), normal(otherNormal), texture(otherTexture)
  {
  }

  bool Vertex::operator==(const Vertex& other) const
  {
    return isEqual(pos, other.pos);
  }

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

  void UniqueVertex::updatePos(const glm::vec3& otherPos)
  {
    pos = otherPos;
    for (auto& originalVertexData : originalVertices)
    {
      originalVertexData.vertex->pos = pos;
    }
  }

  void UniqueVertex::updateNormal(const glm::vec3& otherNormal)
  {
    normal = otherNormal;
  }
}  // namespace MeshCore