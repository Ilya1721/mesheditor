#pragma once

#include "Texture2D.h"

namespace RenderSystem
{
  class TAAMotionVectorsTexture : public Texture2D
  {
  public:
    TAAMotionVectorsTexture();
    TAAMotionVectorsTexture(int width, int height);

    void setDimensions(int width, int height) override;
    int getAttachmentId() const override;

  private:
    void init();
  };
}
