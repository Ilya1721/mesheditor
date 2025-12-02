#include "TAAResolveController.h"

namespace RenderSystem
{
  TAAResolveController::TAAResolveController(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : mShaderProgram(vertexShaderPath, fragmentShaderPath),
      mPrevView(1.0f),
      mCurrentView(1.0f),
      mPrevJitteredProjection(1.0f),
      mCurrentJitteredProjection(1.0f),
      isFirstFrame(true)
  {
  }

  void TAAResolveController::setCurrentColorTexture(const TAAColorTexture& texture) const
  {
    mShaderProgram.setCurrentColorTexture(texture);
  }

  void TAAResolveController::setPreviousColorTexture(const TAAColorTexture& texture) const
  {
    mShaderProgram.setPreviousColorTexture(texture);
  }

  void TAAResolveController::setDepthMap(const DepthTexture& texture) const
  {
    mShaderProgram.setDepthMap(texture);
  }

  void TAAResolveController::setMotionVectorsTexture(
    const TAAMotionVectorsTexture& texture
  ) const
  {
    mShaderProgram.setMotionVectorsTexture(texture);
  }

  void TAAResolveController::setPrevView(const glm::mat4& view) { mPrevView = view; }

  void TAAResolveController::setCurrentView(const glm::mat4& view)
  {
    mCurrentView = view;
  }

  void TAAResolveController::setPrevJitteredProjection(const glm::mat4& projection)
  {
    mPrevJitteredProjection = projection;
  }

  void TAAResolveController::setCurrentJitteredProjection(const glm::mat4& projection)
  {
    mCurrentJitteredProjection = projection;
  }

  void TAAResolveController::setIsFirstFrame(bool isFirstFrame)
  {
    mShaderProgram.setIsFirstFrame(isFirstFrame);
  }

  void TAAResolveController::render(const std::function<void()>& renderFunc) const
  {
    mShaderProgram.invoke(renderFunc);
  }

  void TAAResolveController::calcViewProjMatrices()
  {
    auto preViewProj = mPrevJitteredProjection * mPrevView;
    auto invCurrViewProj = glm::inverse(mCurrentJitteredProjection * mCurrentView);
    mShaderProgram.setPrevViewProj(preViewProj);
    mShaderProgram.setInvCurrViewProj(invCurrViewProj);
  }
}  // namespace RenderSystem
