#include "TAADepthMapController.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  TAADepthMapController::TAADepthMapController(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : mShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
  }

  const DepthTexture& TAADepthMapController::getPrevDepthMap() const
  {
    return mPrevDepthMap;
  }

  const DepthTexture& TAADepthMapController::getCurrDepthMap() const
  {
    return mCurrDepthMap;
  }

  void TAADepthMapController::setScreenSize(int width, int height)
  {
    mPrevDepthMap.create(width, height);
    mCurrDepthMap.create(width, height);
  }

  void TAADepthMapController::renderSceneToDepthMap(
    const std::function<void()>& renderSceneFunc
  )
  {
    mFBO.attachTexture(
      mCurrDepthMap,
      []()
      {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
      }
    );
    mFBO.invoke(
      [this, &renderSceneFunc]()
      {
        glViewport(0, 0, mCurrDepthMap.getWidth(), mCurrDepthMap.getHeight());
        glClear(GL_DEPTH_BUFFER_BIT);
        mShaderProgram.invoke(renderSceneFunc);
      }
    );
  }

  void TAADepthMapController::swapDepthMaps() { mCurrDepthMap.swap(mPrevDepthMap); }

  void TAADepthMapController::setModel(const glm::mat4& model)
  {
    mShaderProgram.setModel(model);
  }

  void TAADepthMapController::setView(const glm::mat4& view)
  {
    mShaderProgram.setView(view);
  }

  void TAADepthMapController::setProjection(const glm::mat4& projection)
  {
    mShaderProgram.setProjection(projection);
  }
}  // namespace RenderSystem
