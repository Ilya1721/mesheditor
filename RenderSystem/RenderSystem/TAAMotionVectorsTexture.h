#pragma once

#include "Texture2D.h"

namespace RenderSystem
{
  class TAAMotionVectorsTexture : public Texture2D
  {
  public:
    TAAMotionVectorsTexture() = default;
    TAAMotionVectorsTexture(int width, int height);

    int getAttachmentId() const override;

    void create(int width, int height) override;
  };
}
