#include "DepthTexture.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  DepthTexture::DepthTexture(int width, int height) : Texture(width, height) { init(); }

  void DepthTexture::setDimensions(int width, int height)
  {
    invoke(
      [&width, &height, this]()
      {
        glTexImage2D(
          GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT,
          GL_FLOAT, 0
        );
        mWidth = width;
        mHeight = height;
      }
    );
  }

  void DepthTexture::init()
  {
    invoke(
      [this]()
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }
    );
  }
}  // namespace RenderSystem
