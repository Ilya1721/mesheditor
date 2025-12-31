#include "ShadowShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "Camera.h"
#include "glad/glad.h"

namespace RenderSystem
{
  ShadowShaderProgram::ShadowShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void ShadowShaderProgram::onCameraPosChanged(Camera* camera)
  {
    invoke(
      [this, camera]()
      { glUniformMatrix4fv(mView, 1, false, glm::value_ptr(camera->getViewMatrix())); }
    );
  }

  void ShadowShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void ShadowShaderProgram::setShadowMap(const DepthTexture& texture) const
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mShadowMap, 0); });
  }

  void ShadowShaderProgram::setLightView(const glm::mat4& lightView)
  {
    invoke([this, &lightView]()
           { glUniformMatrix4fv(mLightView, 1, false, glm::value_ptr(lightView)); });
  }

  void ShadowShaderProgram::setLightProjection(const glm::mat4& lightProjection)
  {
    invoke(
      [this, &lightProjection]()
      { glUniformMatrix4fv(mLightProjection, 1, false, glm::value_ptr(lightProjection)); }
    );
  }

  void ShadowShaderProgram::setShadowBias(float shadowBias)
  {
    invoke([this, &shadowBias]() { glUniform1f(mShadowBias, shadowBias); });
  }

  void ShadowShaderProgram::setProjection(const glm::mat4& projection)
  {
    invoke([this, &projection]()
           { glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection)); });
  }

  void ShadowShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mLightView = getUniformLocation("lightView");
    mLightProjection = getUniformLocation("lightProjection");
    mShadowBias = getUniformLocation("shadowBias");
    mShadowMap = getUniformLocation("shadowMap");
  }
}  // namespace RenderSystem
