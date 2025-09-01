#pragma once

#include "Texture.h"

#include <string>

namespace RenderSystem
{
  class ImageTexture : public Texture
  {
   public:
    ImageTexture() = default;
    ImageTexture(int width, int height);
    ImageTexture(const std::string& filePath);

    void setDimensions(int width, int height) override;

  private:
    unsigned char* mData = nullptr;
  };
}  // namespace RenderSystem
