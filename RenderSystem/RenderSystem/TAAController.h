#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "TAADepthMapController.h"
#include "TAAMotionVectorsController.h"
#include "ViewportListener.h"

namespace RenderSystem
{
  class SceneShaderProgram;

  class TAAController : public CameraListener, public ViewportListener
  {
   public:
    TAAController(
      SceneShaderProgram* sceneShaderProgram,
      const path& depthMapVertexShaderPath,
      const path& depthMapFragmentShaderPath,
      const path& motionVectorsVertexShaderPath,
      const path& motionVectorsFragmentShaderPath
    );

    void onCameraPosChanged(Camera* camera) override;
    void onViewportChanged(Viewport* viewport) override;

    void resetFrameIndex();
    void makeJitteredProjection();
    void setModel(const glm::mat4& model);
    void renderSceneToTextures(const std::function<void()>& renderSceneFunc);

   private:
    void setView(const glm::mat4& view);
    void updateViewportParams(
      const glm::mat4& projection, int screenWidth, int screenHeight
    );
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
