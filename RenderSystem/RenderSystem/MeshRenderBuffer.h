#pragma once

#include "MeshRenderData.h"
#include "RenderBuffer.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

namespace RenderSystem
{
  class MeshRenderBuffer : public RenderBuffer
  {
   public:
    void loadRenderData(const MeshRenderData& renderData);
    void bind() const override;

   private:
    VertexBufferObject mVBO;
    VertexArrayObject mVAO;
  };
}
