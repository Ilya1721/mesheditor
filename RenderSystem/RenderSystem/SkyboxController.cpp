#include "SkyboxController.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
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
    init();
  }

  void SkyboxController::setView(const glm::mat4& view) { mShaderProgram.setView(view); }

  void SkyboxController::setProjection(const glm::mat4& projection)
  {
    mShaderProgram.setProjection(projection);
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

  void SkyboxController::init() { mShaderProgram.setSkyboxCubemap(mCubemapTexture); }
}  // namespace RenderSystem
