#pragma once

#include <glm/glm.hpp>

#include "RenderData.h"

namespace GeometryCore
{
  struct Ray;
  struct Line;
  struct Plane;
}  // namespace GeometryCore

namespace MeshCore
{
  struct Vertex;
  struct OriginalVertexData;
}  // namespace MeshCore

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

    static MeshRenderData createRenderData(const glm::vec3& point);
    static MeshRenderData createRenderData(const GeometryCore::Ray& ray, float length);
    static MeshRenderData createRenderData(
      const GeometryCore::Line& line, bool withArrowHead
    );
    static MeshRenderData createRenderData(
      const GeometryCore::Plane& plane, float width, float length
    );
    static MeshRenderData getSkyboxRenderData();
  };
}
