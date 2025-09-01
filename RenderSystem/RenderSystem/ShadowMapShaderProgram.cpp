#include "ShadowMapShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  ShadowMapShaderProgram::ShadowMapShaderProgram(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath),
      mModel(),
      mLightView(),
      mLightProjection()
  {
    initUniformLocations();
  }

  void ShadowMapShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void ShadowMapShaderProgram::setLightView(const glm::mat4& lightView)
  {
    invoke([this, &lightView]()
           { glUniformMatrix4fv(mLightView, 1, false, glm::value_ptr(lightView)); });
  }

  void ShadowMapShaderProgram::setLightProjection(const glm::mat4& lightProjection)
  {
    invoke(
      [this, &lightProjection]()
      { glUniformMatrix4fv(mLightProjection, 1, false, glm::value_ptr(lightProjection)); }
    );
  }

  void ShadowMapShaderProgram::initUniformLocations()
  {
    mModel = glGetUniformLocation(mShaderProgram, "model");
    mLightView = glGetUniformLocation(mShaderProgram, "lightView");
    mLightProjection = glGetUniformLocation(mShaderProgram, "lightProjection");
  }
}  // namespace RenderSystem
