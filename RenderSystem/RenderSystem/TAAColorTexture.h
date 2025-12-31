#pragma once

#include "Texture2D.h"

namespace RenderSystem
{
  class TAAColorTexture : public Texture2D
  {
  public:
    int getAttachmentId() const override;

    void create(int width, int height) override;
  };
}

