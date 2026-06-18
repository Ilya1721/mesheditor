#include "ShadowShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>
#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"

namespace
{
  constexpr int SHADOW_MAP_UNIT = 0;
}

namespace RenderSystem
{
  ShadowShaderProgram::ShadowShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : Object3DShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void ShadowShaderProgram::preRenderSetup() const
  {
    glBindTextureUnit(SHADOW_MAP_UNIT, mShadowMapId);
  }

  void ShadowShaderProgram::onCameraChanged(const Camera* camera) const
  {
    bind();
    glUniformMatrix4fv(mView, 1, false, glm::value_ptr(camera->getViewMatrix()));
  }

  void ShadowShaderProgram::setModel(const glm::mat4& model) const
  {
    bind();
    glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model));
  }

  void ShadowShaderProgram::setShadowMap(const Texture2D& texture) const
  {
    bind();
    mShadowMapId = texture.getId();
    glUniform1i(mShadowMapLocation, SHADOW_MAP_UNIT);
  }

  void ShadowShaderProgram::setLightView(const glm::mat4& lightView) const
  {
    bind();
    glUniformMatrix4fv(mLightView, 1, false, glm::value_ptr(lightView));
  }

  void ShadowShaderProgram::setLightProjection(const glm::mat4& lightProjection) const
  {
    bind();
    glUniformMatrix4fv(mLightProjection, 1, false, glm::value_ptr(lightProjection));
  }

  void ShadowShaderProgram::setShadowBias(float shadowBias) const
  {
    bind();
    glUniform1f(mShadowBias, shadowBias);
  }

  void ShadowShaderProgram::setProjection(const glm::mat4& projection) const
  {
    bind();
    glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
  }

  void ShadowShaderProgram::setUseSkinningTransform(bool useSkinningTransform) const
  {
    bind();
    glUniform1i(mUseSkinningTransform, useSkinningTransform);
  }

  void ShadowShaderProgram::setSkinningTransforms(
    const std::vector<glm::mat4>& skinningTransforms
  ) const
  {
    if (!skinningTransforms.empty())
    {
      bind();
      glUniformMatrix4fv(
        mSkinningTransforms,
        skinningTransforms.size(),
        GL_FALSE,
        glm::value_ptr(skinningTransforms[0])
      );
    }
  }

  void ShadowShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mLightView = getUniformLocation("lightView");
    mLightProjection = getUniformLocation("lightProjection");
    mShadowBias = getUniformLocation("shadowBias");
    mShadowMapLocation = getUniformLocation("shadowMap");
    mUseSkinningTransform = getUniformLocation("useSkinningTransform");
    mSkinningTransforms = getUniformLocation("skinningTransforms");
  }
}  // namespace RenderSystem
