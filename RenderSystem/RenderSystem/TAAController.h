#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#include "TAAColorBufferController.h"
#include "TAADepthMapController.h"
#include "TAAMotionVectorsController.h"
#include "TAAResolveController.h"
#include "ViewportListener.h"

struct ObjectModel
{
  glm::mat4 prevModel;
  glm::mat4 currentModel;
};

namespace RenderSystem
{
  class SceneShaderProgram;
  class Object3D;

  class TAAController : public ViewportListener
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

    void onViewportChanged(Viewport* viewport) override;

    void resetFrameIndex();
    void makeJitteredProjection();
    void setModel(const Object3D* object, const glm::mat4& model);
    void setView(const glm::mat4& view);
    void renderSceneToDepthMap(const std::function<void()>& renderSceneFunc);
    void renderSceneToMotionVectorsTexture(const std::function<void()>& renderSceneFunc);
    void renderSceneToColorBuffer(const std::function<void()>& renderSceneFunc);
    const TAAColorTexture& resolveTAA(const std::function<void()>& renderFunc);

   private:
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
    bool mIsFirstFrame;
    SceneShaderProgram* mSceneShaderProgram;
    TAADepthMapController mDepthMapController;
    TAAMotionVectorsController mMotionVectorsController;
    TAAColorBufferController mColorBufferController;
    TAAResolveController mResolveController;
    std::unordered_map<const Object3D*, ObjectModel> mObjectModelMap;
  };
}  // namespace RenderSystem
