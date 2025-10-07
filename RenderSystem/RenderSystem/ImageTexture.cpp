#include "ImageTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"
#include "stb_image/stb_image.h"

namespace RenderSystem
{
  ImageTexture::ImageTexture(int width, int height) : Texture2D(width, height)
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

  ImageTexture::ImageTexture(ImageTexture&& other) noexcept { *this = std::move(other); }

  ImageTexture& ImageTexture::operator=(ImageTexture&& other) noexcept
  {
    if (this != &other)
    {
      Texture2D::operator=(std::move(other));
      mData = other.mData;
      mColorChannels = other.mColorChannels;
      other.mData = nullptr;
      other.mColorChannels = 0;
    }

    return *this;
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

  bool ImageTexture::isEmpty() const { return mWidth == 0 || mHeight == 0; }

  ImageTexture::ImageTexture(const std::string& filePath) : Texture2D()
  {
    if (filePath.empty()) { return; }
    mData = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mColorChannels, 0);
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
