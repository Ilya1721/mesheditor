#include "MeshRenderData.h"

#include "MeshCore/MeshFactory.h"
#include "MeshCore/Vertex.h"

namespace RenderSystem
{
  void MeshRenderData::append(const MeshRenderData& renderData)
  {
    RenderData::append(renderData);
  }

  void MeshRenderData::append(const Vertex& vertex)
  {
    for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
    {
      mCompactData.push_back(vertex.pos[coordIdx]);
    }
    for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
    {
      mCompactData.push_back(vertex.normal[coordIdx]);
    }
    for (int coordIdx = 0; coordIdx < 2; ++coordIdx)
    {
      mCompactData.push_back(vertex.texture[coordIdx]);
    }
    for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
    {
      mCompactData.push_back(vertex.color[coordIdx]);
    }
    for (int coordIdx = 0; coordIdx < 4; ++coordIdx)
    {
      mCompactData.push_back(vertex.tangent[coordIdx]);
    }
    for (int coordIdx = 0; coordIdx < 4; ++coordIdx)
    {
      mCompactData.push_back(vertex.weights[coordIdx]);
    }
    for (int coordIdx = 0; coordIdx < 4; ++coordIdx)
    {
      mCompactData.push_back(vertex.jointIndices[coordIdx]);
    }
  }

  void MeshRenderData::updateVertex(
    const OriginalVertexData& vertexData, int startVertexOffset
  )
  {
    unsigned int compactDataIdx = 23 * (startVertexOffset + vertexData.index);
    for (unsigned int coordIdx = 0; coordIdx < 3; ++compactDataIdx, ++coordIdx)
    {
      mCompactData[compactDataIdx] = vertexData.vertex->pos[coordIdx];
      mCompactData[compactDataIdx + 3ull] = vertexData.vertex->normal[coordIdx];
    }
  }

  int MeshRenderData::getVertexCount() const
  {
    return mCompactData.size() / 23;
  }

  MeshRenderData MeshRenderData::generateRenderData(
    const std::vector<Vertex>& vertices, const glm::mat4& transform
  )
  {
    MeshRenderData renderData;
    for (const auto& vertex : vertices)
    {
      renderData.append(transform * vertex);
    }

    return renderData;
  }
}
