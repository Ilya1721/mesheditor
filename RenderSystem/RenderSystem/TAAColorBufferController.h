#pragma once

#include "FrameBufferObject.h"
#include "TAAColorTexture.h"

namespace RenderSystem
{
  class SceneShaderProgram;

  class TAAColorBufferController
  {
   public:
    TAAColorBufferController(SceneShaderProgram* sceneShaderProgram);

    const TAAColorTexture& getCurrentColorBuffer() const;
    const TAAColorTexture& getPreviousColorBuffer() const;

    void setScreenSize(int width, int height);
    void renderSceneToColorBuffers(const std::function<void()>& renderSceneFunc);
    void swapPrevWithResolvedTexture(TAAColorTexture& texture);

  private:
    void init();

  private:
    FrameBufferObject mFBO;
    TAAColorTexture mCurrentColorBuffer;
    TAAColorTexture mPreviousColorBuffer;
    SceneShaderProgram* mSceneShaderProgram;
  };
}  // namespace RenderSystem
