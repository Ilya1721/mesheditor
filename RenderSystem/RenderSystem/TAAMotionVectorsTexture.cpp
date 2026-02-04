#include "TAAMotionVectorsTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  TAAMotionVectorsTexture::TAAMotionVectorsTexture(int width, int height)
    : Texture2D(width, height)
  {
    create(width, height);
  }

  void TAAMotionVectorsTexture::create(int width, int height)
  {
    invoke(
      [&width, &height, this]()
      {
        mWidth = width;
        mHeight = height;

        glTexImage2D(
          GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, nullptr
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }
    );
  }
}  // namespace RenderSystem
