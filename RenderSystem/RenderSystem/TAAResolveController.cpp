#include "TAAResolveController.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

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
      mCurrentJitteredProjection(1.0f)
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

  void TAAResolveController::setScreenSize(int width, int height)
  {
    mResolvedColorTexture.setDimensions(width, height);
    mFBO.attachDepthBuffer(width, height);
    mShaderProgram.setScreenSize(glm::vec2(width, height));
  }

  void TAAResolveController::setIsFirstFrame(bool isFirstFrame)
  {
    mShaderProgram.setIsFirstFrame(isFirstFrame);
  }

  TAAColorTexture& TAAResolveController::render(const std::function<void()>& renderFunc)
  {
    mFBO.attachTexture(
      mResolvedColorTexture,
      [this]() { glDrawBuffer(mResolvedColorTexture.getAttachmentId()); }
    );
    mFBO.invoke(
      [this, &renderFunc]()
      {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mShaderProgram.invoke(renderFunc);
      }
    );

    return mResolvedColorTexture;
  }

  void TAAResolveController::calcViewProjMatrices()
  {
    auto preViewProj = mPrevJitteredProjection * mPrevView;
    auto invCurrViewProj = glm::inverse(mCurrentJitteredProjection * mCurrentView);
    mShaderProgram.setPrevViewProj(preViewProj);
    mShaderProgram.setInvCurrViewProj(invCurrViewProj);
  }
}  // namespace RenderSystem
