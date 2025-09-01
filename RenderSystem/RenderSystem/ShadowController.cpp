#include "ShadowController.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  ShadowController::ShadowController(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : mShaderProgram(vertexShaderPath, fragmentShaderPath), mTexture(0, 0), mFBO()
  {
    init();
  }

  const DepthTexture& ShadowController::getDepthMap() const
  {
    return mTexture;
  }

  ShadowMapShaderProgram* ShadowController::getShaderProgram() { return &mShaderProgram; }

  void ShadowController::setTextureDimensions(int width, int height)
  {
    mTexture.setDimensions(width, height);
  }

  void ShadowController::renderSceneToDepthMap(
    const std::function<void()>& renderSceneFunc
  )
  {
    mFBO.invoke(
      [this, &renderSceneFunc]()
      {
        glViewport(0, 0, mTexture.getWidth(), mTexture.getHeight());
        glClear(GL_DEPTH_BUFFER_BIT);
        mShaderProgram.invoke(renderSceneFunc);
      }
    );
  }

  void ShadowController::init()
  {
    mFBO.attachTexture(
      mTexture,
      []()
      {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
      }
    );
  }
}  // namespace RenderSystem
