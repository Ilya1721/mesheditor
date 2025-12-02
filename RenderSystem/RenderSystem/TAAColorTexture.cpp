#include "TAAColorTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  TAAColorTexture::TAAColorTexture() { init(); }

  int TAAColorTexture::getAttachmentId() const { return GL_COLOR_ATTACHMENT0; }

  void TAAColorTexture::setDimensions(int width, int height)
  {
    invoke(
      [&width, &height, this]()
      {
        glTexImage2D(
          GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr
        );
        mWidth = width;
        mHeight = height;
      }
    );
  }

  void TAAColorTexture::init()
  {
    invoke(
      []()
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
    );
  }
}  // namespace RenderSystem