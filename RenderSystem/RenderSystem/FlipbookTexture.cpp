#include "FlipbookTexture.h"

#include <glad/glad.h>

#include "ImageLoader.h"

namespace RenderSystem
{
  FlipbookTexture::FlipbookTexture(const std::string& filePath) : Texture2D()
  {
    loadImage(
      filePath,
      [this](int width, int height, const unsigned char* decodedData, int colorChannels)
      { create(width, height, decodedData, colorChannels); }
    );
  }

  void FlipbookTexture::create(
    int width, int height, const unsigned char* data, int colorChannels
  )
  {
    invoke(
      [&width, &height, &data, &colorChannels, this]()
      {
        const auto colorFormat = getColorFormat(colorChannels);
        mWidth = width;
        mHeight = height;

        glTexImage2D(
          GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE,
          data
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
    );
  }
}
