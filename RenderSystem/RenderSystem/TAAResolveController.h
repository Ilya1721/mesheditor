#pragma once

#include "TAAResolveShaderProgram.h"
#include "TAAColorTexture.h"

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
    void setIsFirstFrame(bool isFirstFrame);

    void render(const std::function<void()>& renderFunc) const;
    void calcViewProjMatrices();

   private:
    TAAResolveShaderProgram mShaderProgram;
    glm::mat4 mPrevView;
    glm::mat4 mCurrentView;
    glm::mat4 mPrevJitteredProjection;
    glm::mat4 mCurrentJitteredProjection;
    bool isFirstFrame;
  };
}  // namespace RenderSystem
