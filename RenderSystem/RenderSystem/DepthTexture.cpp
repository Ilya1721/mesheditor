#include "DepthTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

namespace RenderSystem
{
  DepthTexture::DepthTexture(int width, int height) : Texture(width, height) { init(); }

  void DepthTexture::setDimensions(int width, int height)
  {
    invokeEditAction(
      [&width, &height]()
      {
        glTexImage2D(
          GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT,
          GL_FLOAT, 0
        );
      }
    );
  }

  void DepthTexture::init()
  {
    invokeEditAction(
      [this]()
      {
        setDimensions(mWidth, mHeight);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }
    );
  }
}  // namespace RenderSystem
