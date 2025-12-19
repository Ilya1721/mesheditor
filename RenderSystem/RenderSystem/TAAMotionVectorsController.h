#pragma once

#include "FrameBufferObject.h"
#include "TAAMotionVectorsShaderProgram.h"
#include "TAAMotionVectorsTexture.h"

namespace RenderSystem
{
  class TAAMotionVectorsController
  {
   public:
    TAAMotionVectorsController(
      const path& vertexShaderPath, const path& fragmentShaderPath
    );

    TAAMotionVectorsShaderProgram* getShaderProgram();
    const TAAMotionVectorsTexture& getMotionVectorsTexture() const;

    void setScreenSize(int width, int height);
    void setPrevModel(const glm::mat4& model);
    void setCurrentModel(const glm::mat4& model);
    void setPrevView(const glm::mat4& view);
    void setCurrentView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);
    void renderSceneToTexture(const std::function<void()>& renderSceneFunc);
    void resetViewModel(const glm::mat4& view, const glm::mat4& model);

   private:
    void init();

   private:
    TAAMotionVectorsTexture mTexture;
    FrameBufferObject mFBO;
    TAAMotionVectorsShaderProgram mShaderProgram;
  };
}  // namespace RenderSystem
