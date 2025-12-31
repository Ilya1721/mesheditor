#include "ControllerWithDepthMap.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "ShaderProgram.h"
#include "glad/glad.h"

namespace RenderSystem
{
  ControllerWithDepthMap& ControllerWithDepthMap::operator=(ShaderProgram* shaderProgram)
  {
    mShaderProgram = shaderProgram;
    init();
    return *this;
  }

  const DepthTexture& ControllerWithDepthMap::getDepthMap() const { return mTexture; }

  void ControllerWithDepthMap::setDepthMapSize(int width, int height)
  {
    mTexture.create(width, height);
  }

  void ControllerWithDepthMap::renderSceneToDepthMap(
    const std::function<void()>& renderSceneFunc
  )
  {
    mFBO.invoke(
      [this, &renderSceneFunc]()
      {
        glViewport(0, 0, mTexture.getWidth(), mTexture.getHeight());
        glClear(GL_DEPTH_BUFFER_BIT);
        mShaderProgram->invoke(renderSceneFunc);
      }
    );
  }

  void ControllerWithDepthMap::init()
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