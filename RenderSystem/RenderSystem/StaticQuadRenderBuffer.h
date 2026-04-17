#pragma once

#include "RequiresBindBeforeInvoke.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

namespace RenderSystem
{
  class StaticQuadRenderBuffer : public RequiresBindBeforeInvoke
  {
   public:
    void loadRenderData(const std::vector<float>& renderData);

   private:
    void bind() const override;
    void unbind() const override;

   private:
    VertexBufferObject mVBO;
    VertexArrayObject mVAO;
  };
}
