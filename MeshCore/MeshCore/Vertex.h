#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_set>

namespace MeshCore
{
  struct HalfEdge;

  struct Vertex
  {
    Vertex() = default;
    Vertex(
      const glm::vec3& otherPos,
      const glm::vec3& otherNormal,
      const glm::vec2& otherTexture = {}
    );

    glm::vec3 pos {};
    glm::vec3 normal {};
    glm::vec2 texture {};
    glm::vec4 tangent {1.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 weights = {1.0f, 0.0f, 0.0f, 0.0f};
    glm::ivec4 jointIndices {0, 0, 0, 0};
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

    void updatePos(const glm::vec3& otherPos);
    void updateNormal(const glm::vec3& otherNormal);

    std::vector<OriginalVertexData> originalVertices;
    std::unordered_set<glm::vec3> adjacentFacesNormals;
  };

  using HalfEdgeVerticesPair = std::pair<Vertex, Vertex>;
}  // namespace MeshCore
