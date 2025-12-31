#include "TAAColorBufferController.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  const TAAColorTexture& TAAColorBufferController::getCurrentColorBuffer() const
  {
    return mCurrentColorBuffer;
  }

  const TAAColorTexture& TAAColorBufferController::getPreviousColorBuffer() const
  {
    return mPreviousColorBuffer;
  }

  void TAAColorBufferController::setScreenSize(int width, int height)
  {
    mCurrentColorBuffer.create(width, height);
    mPreviousColorBuffer.create(width, height);
    mFBO.attachTexture(
      mCurrentColorBuffer,
      [this]() { glDrawBuffer(mCurrentColorBuffer.getAttachmentId()); }
    );
    mFBO.attachDepthBuffer(width, height);
  }

  void TAAColorBufferController::renderSceneToColorBuffer(
    const std::function<void()>& renderSceneFunc
  )
  {
    mFBO.invoke(
      [this, &renderSceneFunc]()
      {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderSceneFunc();
      }
    );
  }

  void TAAColorBufferController::swapPrevWithResolvedTexture(TAAColorTexture& texture)
  {
    mPreviousColorBuffer.swap(texture);
  }
}  // namespace RenderSystem
