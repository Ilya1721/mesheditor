#pragma once

#include "MeshRenderData.h"
#include "RequiresBindBeforeInvoke.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

namespace RenderSystem
{
  class MeshRenderBuffer : public RequiresBindBeforeInvoke
  {
   public:
    void loadRenderData(const MeshRenderData& renderData);

  private:
    void bind() const override;
    void unbind() const override;

  private:
    VertexBufferObject mVBO;
    VertexArrayObject mVAO;
  };
}
