#pragma once

#include "Texture2D.h"

namespace RenderSystem
{
  class TAAMotionVectorsTexture : public Texture2D
  {
  public:
    TAAMotionVectorsTexture() = default;
    TAAMotionVectorsTexture(int width, int height);

    void create(int width, int height);
  };
}
