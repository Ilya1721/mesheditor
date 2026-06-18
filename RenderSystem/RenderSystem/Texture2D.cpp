#include "Texture2D.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include <algorithm>

namespace RenderSystem
{
  Texture2D::Texture2D(int width, int height) : mTexture(), mWidth(width), mHeight(height)
  {
    glGenTextures(1, &mTexture);
  }

  Texture2D::~Texture2D()
  {
    if (mTexture != 0)
    {
      glDeleteTextures(1, &mTexture);
    }
  }

  void Texture2D::bind() const
  {
    glBindTexture(GL_TEXTURE_2D, mTexture);
  }

  unsigned int Texture2D::getId() const
  {
    return mTexture;
  }

  int Texture2D::getWidth() const
  {
    return mWidth;
  }

  int Texture2D::getHeight() const
  {
    return mHeight;
  }
}  // namespace RenderSystem
