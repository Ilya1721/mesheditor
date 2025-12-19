#pragma once

#include "FrameBufferObject.h"
#include "TAAColorTexture.h"
#include "TAAResolveShaderProgram.h"

namespace RenderSystem
{
  class TAAResolveController
  {
   public:
    TAAResolveController(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void setCurrentColorTexture(const TAAColorTexture& texture) const;
    void setPreviousColorTexture(const TAAColorTexture& texture) const;
    void setPrevDepthMap(const DepthTexture& texture) const;
    void setCurrDepthMap(const DepthTexture& texture) const;
    void setMotionVectorsTexture(const TAAMotionVectorsTexture& texture) const;
    void setScreenSize(int width, int height);
    void setIsFirstFrame(bool isFirstFrame);

    TAAColorTexture& render(const std::function<void()>& renderFunc);

   private:
    TAAResolveShaderProgram mShaderProgram;
    FrameBufferObject mFBO;
    TAAColorTexture mResolvedColorTexture;
  };
}  // namespace RenderSystem
