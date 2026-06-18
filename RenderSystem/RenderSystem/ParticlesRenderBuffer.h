#pragma once

#include "ParticlesRenderData.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "RenderBuffer.h"

namespace RenderSystem
{
  class ParticlesRenderBuffer : public RenderBuffer
  {
   public:
    void updateParticlesRenderData(const ParticlesRenderData& renderData);
    void loadParticlesRenderData();
    void loadQuadRenderData(const std::vector<float>& renderData);
    void bind() const override;

   private:
    VertexBufferObject mQuadVBO;
    VertexBufferObject mParticlesVBO;
    VertexArrayObject mVAO;
  };
}
