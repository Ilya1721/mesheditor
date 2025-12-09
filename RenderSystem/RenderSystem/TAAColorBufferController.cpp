#include "TAAColorBufferController.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "SceneShaderProgram.h"
#include "glad/glad.h"

namespace RenderSystem
{
  TAAColorBufferController::TAAColorBufferController(
    SceneShaderProgram* sceneShaderProgram
  )
    : mSceneShaderProgram(sceneShaderProgram)
  {
    init();
  }

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
    mCurrentColorBuffer.setDimensions(width, height);
    mPreviousColorBuffer.setDimensions(width, height);
    mFBO.attachDepthBuffer(width, height);
  }

  void TAAColorBufferController::renderSceneToColorBuffers(
    const std::function<void()>& renderSceneFunc
  )
  {
    mFBO.invoke(
      [this, &renderSceneFunc]()
      {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mSceneShaderProgram->invoke(renderSceneFunc);
      }
    );
  }

  void TAAColorBufferController::init()
  {
    mFBO.attachTexture(
      mCurrentColorBuffer,
      [this]() { glDrawBuffer(mCurrentColorBuffer.getAttachmentId()); }
    );
  }

  void TAAColorBufferController::swapPrevWithResolvedTexture(TAAColorTexture& texture)
  {
    mPreviousColorBuffer.swap(texture);
  }
}  // namespace RenderSystem
