#include "FrameBufferObject.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

namespace RenderSystem
{
  FrameBufferObject::FrameBufferObject() : mFBO(), mDepthRBO()
  {
    init();
  }

  void FrameBufferObject::bind() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthRBO);
  }

  void FrameBufferObject::attachTexture(
    const Texture2D& texture,
    int colorAttachment,
    int readBufferMode,
    int drawBufferMode,
    bool attachDepthBuffer
  ) const
  {
    bind();
    texture.bind();
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, colorAttachment, GL_TEXTURE_2D, texture.getId(), 0
    );
    glReadBuffer(readBufferMode);
    glDrawBuffer(drawBufferMode);
    if (attachDepthBuffer)
    {
      glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, texture.getWidth(), texture.getHeight()
      );
      glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRBO
      );
    }
    glViewport(0, 0, texture.getWidth(), texture.getHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void FrameBufferObject::init()
  {
    glGenFramebuffers(1, &mFBO);
    glGenRenderbuffers(1, &mDepthRBO);
  }
}  // namespace RenderSystem
