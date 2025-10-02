#include "Texture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  Texture::Texture() { glGenTextures(1, &mTexture); }

  Texture::Texture(Texture&& other) noexcept { *this = std::move(other); }

  Texture& Texture::operator=(Texture&& other) noexcept
  {
    if (this != &other)
    {
      if (mTexture != 0) { glDeleteTextures(1, &mTexture); }
      mTexture = other.mTexture;
      mResourceToRestore = other.mResourceToRestore;
      other.mTexture = 0;
      other.mResourceToRestore = 0;
    }

    return *this;
  }

  Texture::~Texture()
  {
    if (mTexture != 0) { glDeleteTextures(1, &mTexture); }
  }

  unsigned int Texture::getId() const { return mTexture; }

  void Texture::bind() const
  {
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &mResourceToRestore);
    glBindTexture(GL_TEXTURE_2D, mTexture);
  }

  void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, mResourceToRestore); }
}  // namespace RenderSystem
