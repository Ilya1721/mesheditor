#pragma once

#include <vector>

#include "RenderBuffer.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

namespace RenderSystem
{
  class StaticQuadRenderBuffer : public RenderBuffer
  {
   public:
    void loadRenderData(const std::vector<float>& renderData);
    void bind() const override;

   private:
    VertexBufferObject mVBO;
    VertexArrayObject mVAO;
  };
}
