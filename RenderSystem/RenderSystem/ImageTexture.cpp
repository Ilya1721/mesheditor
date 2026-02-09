#include "ImageTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "Constants.h"
#include "glad/glad.h"
#include "stb_image/stb_image.h"

namespace RenderSystem
{
  ImageTexture::ImageTexture(int width, int height) : Texture2D(width, height)
  {
    create(width, height, nullptr, 4);
  }

  ImageTexture::ImageTexture(ImageTexture&& other) noexcept { *this = std::move(other); }

  ImageTexture& ImageTexture::operator=(ImageTexture&& other) noexcept
  {
    if (this != &other) { Texture2D::operator=(std::move(other)); }
    return *this;
  }

  void ImageTexture::create(int width, int height, unsigned char* data, int colorChannels)
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
    if (filePath.empty()) { return; }
    int colorChannels = 0;
    auto data = stbi_load(filePath.c_str(), &mWidth, &mHeight, &colorChannels, 0);
    create(mWidth, mHeight, data, colorChannels);
    stbi_image_free(data);
  }

  ImageTexture::ImageTexture(const unsigned char* data, int dataLength)
  {
    if (data == nullptr || dataLength <= 0) { return; }
    int width {}, height {}, colorChannels {};
    auto decoded =
      stbi_load_from_memory(data, dataLength, &width, &height, &colorChannels, 0);
    create(width, height, decoded, colorChannels);
    stbi_image_free(decoded);
  }
}  // namespace RenderSystem
