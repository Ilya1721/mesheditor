#pragma once

#include "Texture.h"

namespace RenderSystem
{
  class DepthTexture : public Texture
  {
   public:
    DepthTexture(int width, int height);

    void setDimensions(int width, int height) override;

   private:
    void init();
  };
}  // namespace RenderSystem
