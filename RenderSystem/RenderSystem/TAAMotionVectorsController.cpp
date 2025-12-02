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
  }

  TAAMotionVectorsShaderProgram* TAAMotionVectorsController::getShaderProgram()
  {
    return &mShaderProgram;
  }

  const TAAMotionVectorsTexture& TAAMotionVectorsController::getMotionVectorsTexture() const
  {
    return mTexture;
  }

  void TAAMotionVectorsController::setTextureDimensions(int width, int height)
  {
    mTexture.setDimensions(width, height);
  }

  void TAAMotionVectorsController::setModel(const glm::mat4& model)
  {
    mShaderProgram.setModel(model);
  }

  void TAAMotionVectorsController::setPrevView(const glm::mat4& view)
  {
    mShaderProgram.setPrevView(view);
  }

  void TAAMotionVectorsController::setCurrentView(const glm::mat4& view)
  {
    mShaderProgram.setCurrentView(view);
  }

  void TAAMotionVectorsController::setProjection(const glm::mat4& projection)
  {
    mShaderProgram.setProjection(projection);
  }

  void TAAMotionVectorsController::setJitteredProjection(const glm::mat4& projection)
  {
    mShaderProgram.setJitteredProjection(projection);
  }

  void TAAMotionVectorsController::renderSceneToTexture(const std::function<void()>& renderSceneFunc)
  {
    mFBO.invoke(
      [this, &renderSceneFunc]()
      {
        glViewport(0, 0, mTexture.getWidth(), mTexture.getHeight());
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
