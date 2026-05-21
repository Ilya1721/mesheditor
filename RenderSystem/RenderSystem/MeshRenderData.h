#pragma once

#include <glm/glm.hpp>

#include "RenderData.h"

namespace GeometryCore
{
  struct Ray;
  struct Line;
  struct Plane;
}

namespace MeshCore
{
  struct Vertex;
  struct OriginalVertexData;
}

using namespace MeshCore;

namespace RenderSystem
{
  class MeshRenderData : public RenderData
  {
   public:
    int getVertexCount() const;

    void append(const MeshRenderData& renderData);
    void append(const Vertex& vertex);
    void updateVertex(const OriginalVertexData& vertexData, int startVertexOffset);

    static MeshRenderData generateRenderData(
      const std::vector<Vertex>& vertices, const glm::mat4& transform = glm::mat4(1.0f)
    );
  };
}
