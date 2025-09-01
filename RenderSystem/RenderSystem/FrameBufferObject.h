#pragma once

#include "RequiresBindBeforeInvoke.h"
#include "Texture.h"

namespace RenderSystem
{
  class FrameBufferObject : public RequiresBindBeforeInvoke
  {
   public:
    FrameBufferObject();

    void attachTexture(
      const Texture& texture, const std::function<void()>& textureSetupFunc
    );

   private:
    void init();
    void bind() const override;
    void unbind() const override;

   private:
    unsigned int mFBO;
  };
}  // namespace RenderSystem
