#include "TAAMotionVectorsController.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  TAAMotionVectorsController::TAAMotionVectorsController(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : mShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    init();
  }

  TAAMotionVectorsShaderProgram* TAAMotionVectorsController::getShaderProgram()
  {
    return &mShaderProgram;
  }

  const TAAMotionVectorsTexture& TAAMotionVectorsController::getMotionVectorsTexture() const
  {
    return mTexture;
  }

  void TAAMotionVectorsController::setScreenSize(int width, int height)
  {
    mTexture.setDimensions(width, height);
    mFBO.attachDepthBuffer(width, height);
  }

  void TAAMotionVectorsController::setPrevModel(const glm::mat4& model)
  {
    mShaderProgram.setPrevModel(model);
  }

  void TAAMotionVectorsController::setCurrentModel(const glm::mat4& model)
  {
    mShaderProgram.setCurrentModel(model);
  }

  void TAAMotionVectorsController::setPrevView(const glm::mat4& view)
  {
    mShaderProgram.setPrevView(view);
  }

  void TAAMotionVectorsController::setCurrentView(const glm::mat4& view)
  {
    mShaderProgram.setCurrentView(view);
  }

  void TAAMotionVectorsController::setPrevJitteredProjection(const glm::mat4& projection)
  {
    mShaderProgram.setPrevJitteredProjection(projection);
  }

  void TAAMotionVectorsController::setCurrentJitteredProjection(const glm::mat4& projection)
  {
    mShaderProgram.setCurrentJitteredProjection(projection);
  }

  void TAAMotionVectorsController::renderSceneToTexture(const std::function<void()>& renderSceneFunc)
  {
    mFBO.invoke(
      [this, &renderSceneFunc]()
      {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mShaderProgram.invoke(renderSceneFunc);
      }
    );
  }

  void TAAMotionVectorsController::init()
  {
    mFBO.attachTexture(
      mTexture,
      [this]()
      {
        GLenum attachments[] = {mTexture.getAttachmentId()};
        glDrawBuffers(1, attachments);
      }
    );
  }
}  // namespace RenderSystem
