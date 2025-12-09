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
    void setDepthMap(const DepthTexture& texture) const;
    void setMotionVectorsTexture(const TAAMotionVectorsTexture& texture) const;
    void setPrevView(const glm::mat4& view);
    void setCurrentView(const glm::mat4& view);
    void setPrevJitteredProjection(const glm::mat4& projection);
    void setCurrentJitteredProjection(const glm::mat4& projection);
    void setScreenSize(int width, int height);
    void setIsFirstFrame(bool isFirstFrame);

    TAAColorTexture& render(const std::function<void()>& renderFunc);
    void calcViewProjMatrices();

   private:
    TAAResolveShaderProgram mShaderProgram;
    FrameBufferObject mFBO;
    TAAColorTexture mResolvedColorTexture;
    glm::mat4 mPrevView;
    glm::mat4 mCurrentView;
    glm::mat4 mPrevJitteredProjection;
    glm::mat4 mCurrentJitteredProjection;
  };
}  // namespace RenderSystem
