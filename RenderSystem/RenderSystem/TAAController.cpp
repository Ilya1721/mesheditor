#include "TAAController.h"

#include <glm/gtx/transform.hpp>

#include "Camera.h"
#include "Constants.h"
#include "SceneShaderProgram.h"
#include "Viewport.h"

namespace
{
  float halton(int index, int base)
  {
    float f = 1.0f, r = 0.0f;
    int i = index;
    while (i > 0)
    {
      f /= base;
      r += f * (i % base);
      i /= base;
    }
    return r;
  }

  glm::vec2 getJitter(int frameIndex)
  {
    return glm::vec2(halton(frameIndex, 2), halton(frameIndex, 3)) - 0.5f;
  }

  glm::vec2 convertJitterToNDC(const glm::vec2& jitter, int screenWidth, int screenHeight)
  {
    return glm::vec2(
      (jitter.x * 2.0f) / static_cast<float>(screenWidth),
      (jitter.y * 2.0f) / static_cast<float>(screenHeight)
    );
  }
}  // namespace

namespace RenderSystem
{
  TAAController::TAAController(
    SceneShaderProgram* sceneShaderProgram,
    const path& depthMapVertexShaderPath,
    const path& depthMapFragmentShaderPath,
    const path& motionVectorsVertexShaderPath,
    const path& motionVectorsFragmentShaderPath,
    const path& resolveVertexShaderPath,
    const path& resolveFragmentShaderPath
  )
    : mProjection(1.0f),
      mView(1.0f),
      mModel(1.0f),
      mScreenWidth(0),
      mScreenHeight(0),
      mFrameIndex(0),
      mIsFirstFrame(true),
      mSceneShaderProgram(sceneShaderProgram),
      mDepthMapController(depthMapVertexShaderPath, depthMapFragmentShaderPath),
      mMotionVectorsController(
        motionVectorsVertexShaderPath, motionVectorsFragmentShaderPath
      ),
      mColorBufferController(sceneShaderProgram),
      mResolveController(resolveVertexShaderPath, resolveFragmentShaderPath)
  {
  }

  void TAAController::onViewportChanged(Viewport* viewport)
  {
    updateViewportParams(
      viewport->getProjectionMatrix(), viewport->getWidth(), viewport->getHeight()
    );
  }

  void TAAController::setModel(const glm::mat4& model)
  {
    if (mIsFirstFrame) { mModel = model; }
    mDepthMapController.setModel(model);
    mMotionVectorsController.setPrevModel(mModel);
    mMotionVectorsController.setCurrentModel(model);
    mModel = model;
  }

  void TAAController::setView(const glm::mat4& view)
  {
    if (mIsFirstFrame) { mView = view; }
    mDepthMapController.setView(view);
    mMotionVectorsController.setPrevView(mView);
    mMotionVectorsController.setCurrentView(view);
    mView = view;
  }

  void TAAController::renderSceneToDepthMap(const std::function<void()>& renderSceneFunc)
  {
    mDepthMapController.renderSceneToDepthMap(renderSceneFunc);
  }

  void TAAController::renderSceneToMotionVectorsTexture(
    const std::function<void()>& renderSceneFunc
  )
  {
    mMotionVectorsController.renderSceneToTexture(renderSceneFunc);
  }

  void TAAController::renderSceneToColorBuffer(
    const std::function<void()>& renderSceneFunc
  )
  {
    mColorBufferController.renderSceneToColorBuffer(renderSceneFunc);
  }

  const TAAColorTexture& TAAController::resolveTAA(const std::function<void()>& renderFunc
  )
  {
    mResolveController.setCurrentColorTexture(
      mColorBufferController.getCurrentColorBuffer()
    );
    mResolveController.setPreviousColorTexture(
      mColorBufferController.getPreviousColorBuffer()
    );
    mResolveController.setPrevDepthMap(mDepthMapController.getPrevDepthMap());
    mResolveController.setCurrDepthMap(mDepthMapController.getCurrDepthMap());
    mResolveController.setMotionVectorsTexture(
      mMotionVectorsController.getMotionVectorsTexture()
    );
    mResolveController.setIsFirstFrame(mIsFirstFrame);
    auto& resolvedTexture = mResolveController.render(renderFunc);
    mColorBufferController.swapPrevWithResolvedTexture(resolvedTexture);
    mDepthMapController.swapDepthMaps();
    mIsFirstFrame = false;

    return mColorBufferController.getPreviousColorBuffer();
  }

  void TAAController::setProjection(const glm::mat4& projection)
  {
    mProjection = projection;
    mDepthMapController.setProjection(projection);
    mMotionVectorsController.setProjection(projection);
  }

  void TAAController::setScreenSize(int screenWidth, int screenHeight)
  {
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;
  }

  void TAAController::updateViewportParams(
    const glm::mat4& stableProjection, int screenWidth, int screenHeight
  )
  {
    resetFrameIndex();
    setProjection(stableProjection);
    setScreenSize(screenWidth, screenHeight);
    mDepthMapController.setScreenSize(screenWidth, screenHeight);
    mMotionVectorsController.setScreenSize(screenWidth, screenHeight);
    mColorBufferController.setScreenSize(screenWidth, screenHeight);
    mResolveController.setScreenSize(screenWidth, screenHeight);
  }

  void TAAController::resetFrameIndex() { mFrameIndex = 0; }

  void TAAController::makeJitteredProjection()
  {
    auto jitter = getJitter(mFrameIndex + 1);
    auto jitterNDC = convertJitterToNDC(jitter, mScreenWidth, mScreenHeight);
    glm::vec3 jitterTranslationVec {jitterNDC.x, jitterNDC.y, 0.0f};
    auto jitterTranslation = glm::translate(glm::mat4(1.0f), jitterTranslationVec);
    mSceneShaderProgram->setJitteredProjection(jitterTranslation * mProjection);
    mFrameIndex = (mFrameIndex + 1) % SAMPLE_COUNT_TAA;
  }
}  // namespace RenderSystem
