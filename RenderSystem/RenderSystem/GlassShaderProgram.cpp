#include "GlassShaderProgram.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Material.h"

namespace
{
  constexpr int SKYBOX_TEXTURE_UNIT = 0;
}

namespace RenderSystem
{
  GlassShaderProgram::GlassShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : Object3DShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void GlassShaderProgram::preRenderSetup() const
  {
    glBindTextureUnit(SKYBOX_TEXTURE_UNIT, mSkyboxTextureId);
  }

  void GlassShaderProgram::onCameraChanged(const Camera* camera) const
  {
    bind();
    glUniformMatrix4fv(mView, 1, false, glm::value_ptr(camera->getViewMatrix()));
  }

  void GlassShaderProgram::setModel(const glm::mat4& model) const
  {
    bind();
    glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model));
  }

  void GlassShaderProgram::setMaterial(const Material& material) const
  {
    bind();
    const auto& glassMaterial = static_cast<const GlassMaterial&>(material);
    glUniform1f(mRefractiveIndex, glassMaterial.refractiveIndex);
    glUniform1f(mReflectionStrength, glassMaterial.reflectionStrength);
    glUniform1f(mTransparency, glassMaterial.transparency);
    glUniform1f(mInterpolationFactor, glassMaterial.interpolationFactor);
    glUniform3fv(mColor, 1, glm::value_ptr(glassMaterial.color));
  }

  void GlassShaderProgram::setProjection(const glm::mat4& projection) const
  {
    bind();
    glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
  }

  void GlassShaderProgram::setSkyboxCubemap(const CubemapTexture& texture) const
  {
    bind();
    mSkyboxTextureId = texture.getId();
    glUniform1i(mSkyboxTextureLocation, SKYBOX_TEXTURE_UNIT);
  }

  void GlassShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mSkyboxTextureLocation = getUniformLocation("skybox");
    mRefractiveIndex = getUniformLocation("material.refractiveIndex");
    mReflectionStrength = getUniformLocation("material.reflectionStrength");
    mTransparency = getUniformLocation("material.transparency");
    mInterpolationFactor = getUniformLocation("material.interpolationFactor");
    mColor = getUniformLocation("material.color");
  }
}  // namespace RenderSystem
