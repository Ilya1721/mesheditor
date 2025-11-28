#pragma once

#include "Texture2D.h"

namespace RenderSystem
{
  class DepthTexture : public Texture2D
  {
   public:
    DepthTexture();
    DepthTexture(int width, int height);

    void setDimensions(int width, int height) override;
    int getAttachmentId() const override;

   private:
    void init();
  };
}  // namespace RenderSystem
