#include "DepthTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  DepthTexture::DepthTexture(int width, int height) : Texture2D(width, height)
  {
    create(width, height);
  }

  void DepthTexture::create(int width, int height)
  {
    invoke(
      [&width, &height, this]()
      {
        mWidth = width;
        mHeight = height;

        glTexImage2D(
          GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT,
          GL_FLOAT, 0
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
      }
    );
  }

  int DepthTexture::getAttachmentId() const { return GL_DEPTH_ATTACHMENT; }
}  // namespace RenderSystem
