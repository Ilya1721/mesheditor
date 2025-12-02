#include "FrameBufferObject.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  FrameBufferObject::FrameBufferObject() : mFBO(), mDepthRBO() { init(); }

  void FrameBufferObject::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, mFBO); }

  void FrameBufferObject::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

  void FrameBufferObject::attachTexture(
    const Texture& texture, const std::function<void()>& textureSetupFunc
  )
  {
    invoke(
      [&texture, &textureSetupFunc]()
      {
        glFramebufferTexture2D(
          GL_FRAMEBUFFER, texture.getAttachmentId(), GL_TEXTURE_2D, texture.getId(), 0
        );
        textureSetupFunc();
      }
    );
  }

  void FrameBufferObject::attachDepthBuffer(int width, int height)
  {
    invoke(
      [this, width, height]()
      {
        glGenRenderbuffers(1, &mDepthRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRBO);
      }
    );
  }

  void FrameBufferObject::init() { glGenFramebuffers(1, &mFBO); }
}  // namespace RenderSystem
