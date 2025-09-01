#include "Texture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  Texture::Texture(int width, int height) : mWidth(width), mHeight(height) { init(); }

  Texture::~Texture()
  {
    if (mTexture != 0) { glDeleteTextures(1, &mTexture); }
  }

  void Texture::passToFragmentShader(int textureLocation) const
  {
    glActiveTexture(GL_TEXTURE0 + mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glUniform1i(textureLocation, mTexture);
  }

  unsigned int Texture::getId() const { return mTexture; }

  void Texture::init()
  {
    glGenTextures(1, &mTexture);
    invoke(
      []()
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
      }
    );
  }

  void Texture::bind() const
  {
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &mResourceToRestore);
    glBindTexture(GL_TEXTURE_2D, mTexture);
  }

  void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, mResourceToRestore); }

  int Texture::getWidth() const { return mWidth; }

  int Texture::getHeight() const { return mHeight; }
}  // namespace RenderSystem
