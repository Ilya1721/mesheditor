#pragma once

#include "Texture2D.h"

namespace RenderSystem
{
  class TAAColorTexture : public Texture2D
  {
  public:
    void create(int width, int height);
  };
}

