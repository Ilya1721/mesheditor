#include "FrameBufferObject.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  FrameBufferObject::FrameBufferObject() { init(); }

  void FrameBufferObject::bind() { glBindFramebuffer(GL_FRAMEBUFFER, mFBO); }

  void FrameBufferObject::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

  void FrameBufferObject::attachTexture(
    const Texture& texture, const std::function<void()>& textureSetupFunc
  )
  {
    invoke(
      [&texture, &textureSetupFunc]()
      {
        glFramebufferTexture2D(
          GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.getId(), 0
        );
        textureSetupFunc();
      }
    );
  }

  void FrameBufferObject::init() { glGenFramebuffers(1, &mFBO); }
}  // namespace RenderSystem
