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

    const TAAMotionVectorsTexture& getMotionVectorsTexture() const;

    void setTextureDimensions(int width, int height);
    void setModel(const glm::mat4& model);
    void setPrevView(const glm::mat4& view);
    void setCurrentView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);
    void setJitteredProjection(const glm::mat4& projection);
    void renderSceneToTexture(const std::function<void()>& renderSceneFunc);

   private:
    void init();

   private:
    TAAMotionVectorsTexture mTexture;
    FrameBufferObject mFBO;
    TAAMotionVectorsShaderProgram mShaderProgram;
  };
}  // namespace RenderSystem
