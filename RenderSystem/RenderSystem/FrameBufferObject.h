#pragma once

#include <functional>

#include "Texture2D.h"

namespace RenderSystem
{
  class FrameBufferObject
  {
   public:
    FrameBufferObject();

    void attachTexture(
      const Texture2D& texture,
      int colorAttachment,
      int readBufferMode,
      int drawBufferMode,
      bool attachDepthBuffer
    ) const;
    void bind() const;

   private:
    void init();

   protected:
    unsigned int mFBO;
    unsigned int mDepthRBO;
  };
}  // namespace RenderSystem
