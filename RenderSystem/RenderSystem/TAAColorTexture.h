#pragma once

#include "Texture2D.h"

namespace RenderSystem
{
  class TAAColorTexture : public Texture2D
  {
  public:
    TAAColorTexture();

    int getAttachmentId() const override;

    void setDimensions(int width, int height) override;

  private:
    void init();
  };
}

