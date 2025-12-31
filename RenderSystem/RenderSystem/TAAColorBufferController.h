#pragma once

#include "FrameBufferObject.h"
#include "TAAColorTexture.h"

namespace RenderSystem
{
  class ShaderProgram;

  class TAAColorBufferController
  {
   public:
    const TAAColorTexture& getCurrentColorBuffer() const;
    const TAAColorTexture& getPreviousColorBuffer() const;

    void setScreenSize(int width, int height);
    void renderSceneToColorBuffer(const std::function<void()>& renderSceneFunc);
    void swapPrevWithResolvedTexture(TAAColorTexture& texture);

   private:
    FrameBufferObject mFBO;
    TAAColorTexture mCurrentColorBuffer;
    TAAColorTexture mPreviousColorBuffer;
  };
}  // namespace RenderSystem
