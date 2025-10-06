#pragma once

#include <vector>

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
  class RenderData
  {
   public:
    void append(const RenderData& other);
    void append(const Vertex& vertex);
    void updateVertex(const OriginalVertexData& vertexData, int startVertexOffset);

    const std::vector<float>& getCompactData() const;
    int getVertexCount() const;

    static RenderData createRenderData(const GeometryCore::Ray& ray, float length);
    static RenderData createRenderData(
      const GeometryCore::Line& line, bool withArrowHead
    );
    static RenderData createRenderData(
      const GeometryCore::Plane& plane, float width, float length
    );
    static RenderData getSkyboxRenderData();

   private:
    std::vector<float> mCompactData;
  };
}  // namespace RenderSystem
