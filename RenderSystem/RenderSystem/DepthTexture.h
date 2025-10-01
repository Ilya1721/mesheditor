#pragma once

#include "Texture2D.h"

namespace RenderSystem
{
  class DepthTexture : public Texture2D
  {
   public:
    DepthTexture(int width, int height);

    void setDimensions(int width, int height) override;

   private:
    void init();
  };
}  // namespace RenderSystem
