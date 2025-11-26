#include "SkyboxController.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "Camera.h"
#include "SceneShaderProgram.h"
#include "glad/glad.h"
#include "Viewport.h"

namespace RenderSystem
{
  SkyboxController::SkyboxController(
    const path& vertexShader,
    const path& fragmentShader,
    const std::array<path, 6>& cubemapTextures,
    SceneShaderProgram* sceneShaderProgram
  )
    : mShaderProgram(vertexShader, fragmentShader),
      mCubemapTexture(cubemapTextures),
      mSceneShaderProgram(sceneShaderProgram)
  {
    init();
  }

  void SkyboxController::onCameraPosChanged(Camera* camera)
  {
    mShaderProgram.setView(camera->getViewMatrix());
  }

  void SkyboxController::onViewportChanged(Viewport* viewport)
  {
    mShaderProgram.setProjection(viewport->getProjectionMatrix());
  }

  void SkyboxController::render(const std::function<void()>& renderFunc)
  {
    mShaderProgram.invoke(
      [&renderFunc, this]()
      {
        GLint previousDepthFunc;
        glGetIntegerv(GL_DEPTH_FUNC, &previousDepthFunc);
        glDepthFunc(GL_LEQUAL);
        renderFunc();
        glDepthFunc(previousDepthFunc);
      }
    );
  }

  void SkyboxController::init()
  {
    mShaderProgram.setSkyboxCubemap(mCubemapTexture);
    mSceneShaderProgram->setSkyboxCubemap(mCubemapTexture);
  }
}  // namespace RenderSystem
