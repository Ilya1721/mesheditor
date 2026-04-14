#pragma once

#include <string>

#include "Texture2D.h"

namespace RenderSystem
{
  class FlipbookTexture : public Texture2D
  {
   public:
    FlipbookTexture(const std::string& filePath);

   private:
    void create(int width, int height, const unsigned char* data, int colorChannels);
  };
}
