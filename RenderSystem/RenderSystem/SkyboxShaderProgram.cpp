#include "SkyboxShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  SkyboxShaderProgram::SkyboxShaderProgram(
    const path& vertexShader, const path& fragmentShader
  )
    : ShaderProgram(vertexShader, fragmentShader), mView(), mProjection(), mSkybox()
  {
    initUniformLocations();
  }

  void SkyboxShaderProgram::setView(const glm::mat4& view)
  {
    invoke([this, &view]() { glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view)); }
    );
  }

  void SkyboxShaderProgram::setProjection(const glm::mat4& projection)
  {
    invoke([this, &projection]()
           { glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection)); });
  }

  void SkyboxShaderProgram::setSkyboxCubemap(const CubemapTexture& texture) const
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mSkybox); });
  }

  void SkyboxShaderProgram::initUniformLocations()
  {
    mView = glGetUniformLocation(mShaderProgram, "view");
    mProjection = glGetUniformLocation(mShaderProgram, "projection");
    mSkybox = glGetUniformLocation(mShaderProgram, "skybox");
  }
}  // namespace RenderSystem
