#pragma once

#include <string>

#include "Texture2D.h"

namespace RenderSystem
{
  class ImageTexture : public Texture2D
  {
   public:
    ImageTexture(int width, int height);
    ImageTexture(const std::string& filePath);
    ImageTexture(const unsigned char* data, int dataSize);

   private:
    void create(int width, int height, const unsigned char* data, int colorChannels);
  };
}  // namespace RenderSystem
