#include "SkyboxShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"
#include "Constants.h"
#include "Viewport.h"

namespace
{
  constexpr int SKYBOX_UNIT = 0;
}

namespace RenderSystem
{
  SkyboxShaderProgram::SkyboxShaderProgram(
    const path& vertexShader, const path& fragmentShader
  )
    : ShaderProgram(vertexShader, fragmentShader)
  {
    initUniformLocations();
  }

  void SkyboxShaderProgram::preRenderSetup() const
  {
    glBindTextureUnit(SKYBOX_UNIT, mSkyboxId);
  }

  void SkyboxShaderProgram::onCameraChanged(const Camera* camera) const
  {
    bind();
    glUniformMatrix4fv(mView, 1, false, glm::value_ptr(camera->getViewMatrix()));
  }

  void SkyboxShaderProgram::setProjection(const glm::mat4& projection) const
  {
    bind();
    glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
  }

  void SkyboxShaderProgram::setSkyboxCubemap(const CubemapTexture& texture) const
  {
    bind();
    mSkyboxId = texture.getId();
    glUniform1i(mSkyboxLocation, SKYBOX_UNIT);
  }

  void SkyboxShaderProgram::initUniformLocations()
  {
    mView = glGetUniformLocation(mShaderProgram, "view");
    mProjection = glGetUniformLocation(mShaderProgram, "projection");
    mSkyboxLocation = glGetUniformLocation(mShaderProgram, "skybox");
  }
}  // namespace RenderSystem
