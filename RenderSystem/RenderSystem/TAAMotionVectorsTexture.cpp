#include "TAAMotionVectorsTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  TAAMotionVectorsTexture::TAAMotionVectorsTexture() : Texture2D()
  {
    init();
  }

  TAAMotionVectorsTexture::TAAMotionVectorsTexture(int width, int height)
    : Texture2D(width, height)
  {
    init();
  }

  void TAAMotionVectorsTexture::setDimensions(int width, int height)
  {
    invoke(
      [&width, &height, this]()
      {
        glTexImage2D(
          GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, nullptr
        );
        mWidth = width;
        mHeight = height;
      }
    );
  }

  int TAAMotionVectorsTexture::getAttachmentId() const
  {
    return GL_COLOR_ATTACHMENT0;
  }

  void TAAMotionVectorsTexture::init()
  {
    invoke(
      [this]()
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }
    );
  }
}  // namespace RenderSystem
