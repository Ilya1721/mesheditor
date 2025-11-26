#pragma once

#include <glm/glm.hpp>

#include "TAADepthMapController.h"
#include "TAAMotionVectorsController.h"

namespace RenderSystem
{
  class SceneShaderProgram;

  class TAAController
  {
   public:
    TAAController(
      SceneShaderProgram* sceneShaderProgram,
      const path& depthMapVertexShaderPath,
      const path& depthMapFragmentShaderPath,
      const path& motionVectorsVertexShaderPath,
      const path& motionVectorsFragmentShaderPath
    );

    void updateViewportParams(
      const glm::mat4& projection, int screenWidth, int screenHeight
    );
    void resetFrameIndex();
    void makeJitteredProjection();
    void setModel(const glm::mat4& model);
    void setView(const glm::mat4& view);
    void renderSceneToTextures(const std::function<void()>& renderSceneFunc);
    void setDepthMapSize(int width, int height);
    void setMotionVectorsTextureSize(int width, int height);

   private:
    void setProjection(const glm::mat4& projection);
    void setScreenSize(int screenWidth, int screenHeight);

   private:
    glm::mat4 mProjection;
    glm::mat4 mView;
    int mScreenWidth;
    int mScreenHeight;
    int mFrameIndex;
    SceneShaderProgram* mSceneShaderProgram;
    TAADepthMapController mDepthMapController;
    TAAMotionVectorsController mMotionVectorsController;
  };
}  // namespace RenderSystem
