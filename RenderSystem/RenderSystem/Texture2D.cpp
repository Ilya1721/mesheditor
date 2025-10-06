#include "Texture2D.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  Texture2D::Texture2D(int width, int height) : Texture(), mWidth(width), mHeight(height)
  {
    init();
  }

  Texture2D::Texture2D(Texture2D&& other) noexcept { *this = std::move(other); }

  Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
  {
    if (this != &other)
    {
      Texture::operator=(std::move(other));
      mWidth = other.mWidth;
      mHeight = other.mHeight;
      other.mWidth = 0;
      other.mHeight = 0;
    }

    return *this;
  }

  int Texture2D::getWidth() const { return mWidth; }

  int Texture2D::getHeight() const { return mHeight; }

  void Texture2D::passToFragmentShader(int textureLocation, int textureSlot) const
  {
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glUniform1i(textureLocation, textureSlot);
  }

  void Texture2D::init()
  {
    invoke(
      []()
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
      }
    );
  }
}  // namespace RenderSystem