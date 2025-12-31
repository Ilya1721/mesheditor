#include "ImageTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"
#include "stb_image/stb_image.h"
#include "Constants.h"

namespace RenderSystem
{
  ImageTexture::ImageTexture(int width, int height) : Texture2D(width, height)
  {
    create(width, height);
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

  void ImageTexture::create(int width, int height)
  {
    invoke(
      [&width, &height, this]()
      {
        const auto colorFormat = getColorFormat(mColorChannels);
        mWidth = width;
        mHeight = height;

        glTexImage2D(
          GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE,
          mData
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

  int ImageTexture::getAttachmentId() const { return GL_COLOR_ATTACHMENT0; }

  ImageTexture::ImageTexture(const std::string& filePath) : Texture2D()
  {
    if (filePath.empty()) { return; }
    mData = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mColorChannels, 0);
    create(mWidth, mHeight);
    stbi_image_free(mData);
  }
}  // namespace RenderSystem
