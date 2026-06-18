#include "TextureFactory.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Constants.h"
#include "ImageLoader.h"
#include "TextureUtils.h"

namespace
{
  using namespace RenderSystem;

  std::shared_ptr<Texture2D> createImageTexture(
    int width, int height, int colorChannels, const unsigned char* data
  )
  {
    auto texture = std::make_shared<Texture2D>(width, height);
    texture->bind();
    const auto colorFormat = getColorFormat(colorChannels);
    glTexImage2D(
      GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, data
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

    return texture;
  }
}

namespace RenderSystem
{
  std::shared_ptr<Texture2D> createMotionVectorsTexture(int width, int height)
  {
    auto texture = std::make_shared<Texture2D>(width, height);
    texture->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture;
  }

  std::shared_ptr<Texture2D> createColorTexture(int width, int height)
  {
    auto texture = std::make_shared<Texture2D>(width, height);
    texture->bind();
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
  }

  std::shared_ptr<Texture2D> createDepthTexture(int width, int height)
  {
    auto texture = std::make_shared<Texture2D>(width, height);
    texture->bind();
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT,
      GL_FLOAT, 0
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, DEPTH_TEXTURE_BORDER_COLOR);

    return texture;
  }

  std::shared_ptr<Texture2D> createFlipbookTexture(const std::string& filePath)
  {
    ImageLoader flipbook(filePath);
    flipbook.load();
    auto texture = std::make_shared<Texture2D>(flipbook.getWidth(), flipbook.getHeight());
    texture->bind();
    const auto colorFormat = getColorFormat(flipbook.getColorChannels());
    glTexImage2D(
      GL_TEXTURE_2D, 0, colorFormat, flipbook.getWidth(), flipbook.getHeight(), 0,
      colorFormat, GL_UNSIGNED_BYTE, flipbook.getData()
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
  }

  std::shared_ptr<Texture2D> createImageTexture(const std::string& filePath)
  {
    ImageLoader image(filePath);
    image.load();

    return ::createImageTexture(
      image.getWidth(), image.getHeight(), image.getColorChannels(), image.getData()
    );
  }

  std::shared_ptr<Texture2D> createImageTexture(const unsigned char* data, int dataSize)
  {
    ImageLoader image(data, dataSize);
    image.load();

    return ::createImageTexture(
      image.getWidth(), image.getHeight(), image.getColorChannels(), image.getData()
    );
  }
}
