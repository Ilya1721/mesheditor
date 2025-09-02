#include "ImageTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"
#include "stb_image/stb_image.h"

namespace
{
  int getColorFormat(int colorChannels)
  {
    switch (colorChannels)
    {
      case 1: return GL_RED;
      case 3: return GL_RGB;
      case 4: return GL_RGBA;
      default: return GL_RGB;
    }
  }
}

namespace RenderSystem
{
  ImageTexture::ImageTexture(int width, int height) : Texture(width, height)
  {
    setDimensions(mWidth, mHeight);
    invoke(
      []()
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
    );
  }

  void ImageTexture::setDimensions(int width, int height)
  {
    invoke(
      [&width, &height, this]()
      {
        const auto colorFormat = getColorFormat(mColorChannels);
        glTexImage2D(
          GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE,
          mData
        );
      }
    );
  }

  ImageTexture::ImageTexture(const std::string& filePath) : Texture()
  {
    if (filePath.empty()) { return; }
    mData = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mColorChannels, 0);
    glGenTextures(1, &mTexture);
    setDimensions(mWidth, mHeight);
    invoke(
      []()
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
      }
    );
    stbi_image_free(mData);
  }
}  // namespace RenderSystem
