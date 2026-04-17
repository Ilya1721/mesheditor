#pragma once

#include "ParticlesRenderData.h"
#include "RequiresBindBeforeInvoke.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

namespace RenderSystem
{
  class ParticlesRenderBuffer : public RequiresBindBeforeInvoke
  {
   public:
    void updateParticlesRenderData(const ParticlesRenderData& renderData);
    void loadParticlesRenderData();
    void loadQuadRenderData(const std::vector<float>& renderData);

  private:
    void bind() const override;
    void unbind() const override;

   private:
    VertexBufferObject mQuadVBO;
    VertexBufferObject mParticlesVBO;
    VertexArrayObject mVAO;
  };
}
