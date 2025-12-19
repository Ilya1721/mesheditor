#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "TAAColorBufferController.h"
#include "TAADepthMapController.h"
#include "TAAMotionVectorsController.h"
#include "TAAResolveController.h"
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
      const path& motionVectorsFragmentShaderPath,
      const path& resolveVertexShaderPath,
      const path& resolveFragmentShaderPath
    );

    void onCameraPosChanged(Camera* camera) override;
    void onViewportChanged(Viewport* viewport) override;

    void resetFrameIndex();
    void makeJitteredProjection();
    void setModel(const glm::mat4& model);
    void renderSceneToDepthMap(const std::function<void()>& renderSceneFunc);
    void renderSceneToMotionVectorsTexture(const std::function<void()>& renderSceneFunc);
    void renderSceneToColorBuffer(const std::function<void()>& renderSceneFunc);
    const TAAColorTexture& resolveTAA(const std::function<void()>& renderFunc);

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
    glm::mat4 mModel;
    int mScreenWidth;
    int mScreenHeight;
    int mFrameIndex;
    bool mIsFirstFrame;
    SceneShaderProgram* mSceneShaderProgram;
    TAADepthMapController mDepthMapController;
    TAAMotionVectorsController mMotionVectorsController;
    TAAColorBufferController mColorBufferController;
    TAAResolveController mResolveController;
  };
}  // namespace RenderSystem
