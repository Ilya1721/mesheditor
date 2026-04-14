#include "ImageTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Constants.h"
#include "ImageLoader.h"

namespace RenderSystem
{
  ImageTexture::ImageTexture(int width, int height) : Texture2D(width, height)
  {
    create(width, height, nullptr, 4);
  }

  void ImageTexture::create(
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        float maxAnisotropyLevel = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropyLevel);
        float anisotropyLevel = std::min(ANISOTROPIC_FILTERING_LEVEL, maxAnisotropyLevel);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
      }
    );
  }

  ImageTexture::ImageTexture(const std::string& filePath) : Texture2D()
  {
    loadImage(
      filePath,
      [this](int width, int height, const unsigned char* decodedData, int colorChannels)
      { create(width, height, decodedData, colorChannels); }
    );
  }

  ImageTexture::ImageTexture(const unsigned char* data, int dataSize)
  {
    loadImage(
      data, dataSize,
      [this](int width, int height, const unsigned char* decodedData, int colorChannels)
      { create(width, height, decodedData, colorChannels); }
    );
  }
}  // namespace RenderSystem
