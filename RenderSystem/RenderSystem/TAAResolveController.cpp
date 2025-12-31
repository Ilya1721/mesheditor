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
    : mShaderProgram(vertexShaderPath, fragmentShaderPath)
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

  void TAAResolveController::setPrevDepthMap(const DepthTexture& texture) const
  {
    mShaderProgram.setPrevDepthMap(texture);
  }

  void TAAResolveController::setCurrDepthMap(const DepthTexture& texture) const
  {
    mShaderProgram.setCurrDepthMap(texture);
  }

  void TAAResolveController::setMotionVectorsTexture(
    const TAAMotionVectorsTexture& texture
  ) const
  {
    mShaderProgram.setMotionVectorsTexture(texture);
  }

  void TAAResolveController::setScreenSize(int width, int height)
  {
    mResolvedColorTexture.create(width, height);
    mShaderProgram.setScreenSize(glm::vec2(width, height));
    mFBO.attachDepthBuffer(width, height);
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
}  // namespace RenderSystem
