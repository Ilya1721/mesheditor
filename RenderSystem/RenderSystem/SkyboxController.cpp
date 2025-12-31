#include "SkyboxController.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "Camera.h"
#include "Viewport.h"
#include "glad/glad.h"

namespace RenderSystem
{
  SkyboxController::SkyboxController(
    const path& vertexShader,
    const path& fragmentShader,
    const std::array<path, 6>& cubemapTextures
  )
    : mShaderProgram(vertexShader, fragmentShader), mCubemapTexture(cubemapTextures)
  {
  }

  const CubemapTexture& SkyboxController::getCubemapTexture()
  {
    return mCubemapTexture;
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
    mShaderProgram.setSkyboxCubemap(mCubemapTexture);
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
}  // namespace RenderSystem
