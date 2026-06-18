#include "BlinnPhongShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"
#include "Material.h"

namespace
{
  constexpr int DIFFUSE_TEXTURE_UNIT = 0;
}

namespace RenderSystem
{
  BlinnPhongShaderProgram::BlinnPhongShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : Object3DShaderProgram(vertexShaderPath, fragmentShaderPath),
      mModel(),
      mView(),
      mProjection()
  {
    mDirectionalLight.init(mShaderProgram);
    mPointLights.init(mShaderProgram);
    initUniformLocations();
  }

  void BlinnPhongShaderProgram::preRenderSetup() const
  {
    if (getFlagFromShader(mHasDiffuseTexture))
    {
      glBindTextureUnit(DIFFUSE_TEXTURE_UNIT, mDiffuseTextureId);
    }
  }

  void BlinnPhongShaderProgram::onCameraChanged(const Camera* camera) const
  {
    setCameraPos(camera->getEye());
    setView(camera->getViewMatrix());
  }

  void BlinnPhongShaderProgram::setModel(const glm::mat4& model) const
  {
    bind();
    glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model));
  }

  void BlinnPhongShaderProgram::setMaterial(const Material& material) const
  {
    bind();
    const auto& blinnPhongMaterial = static_cast<const BlinnPhongMaterial&>(material);
    glUniform3fv(mAmbient, 1, glm::value_ptr(blinnPhongMaterial.ambient));
    glUniform3fv(mDiffuse, 1, glm::value_ptr(blinnPhongMaterial.diffuse));
    glUniform3fv(mSpecular, 1, glm::value_ptr(blinnPhongMaterial.specular));
    glUniform1f(mShininess, blinnPhongMaterial.shininess);
    glUniform2fv(mUVScale, 1, glm::value_ptr(blinnPhongMaterial.uvScale));
    setDiffuseTexture(blinnPhongMaterial.diffuseTexture.get());
  }

  void BlinnPhongShaderProgram::setDirLightSourcePos(const glm::vec3& lightSourcePos
  ) const
  {
    bind();
    mDirectionalLight.setLightSourcePos(glm::value_ptr(lightSourcePos));
  }

  void BlinnPhongShaderProgram::setCameraPos(const glm::vec3& cameraPos) const
  {
    bind();
    mDirectionalLight.setCameraPos(glm::value_ptr(cameraPos));
  }

  void BlinnPhongShaderProgram::setDirLightParams(const DirectionalLightParams& params
  ) const
  {
    bind();
    mDirectionalLight.setParams(params);
  }

  void BlinnPhongShaderProgram::setView(const glm::mat4& view) const
  {
    bind();
    glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view));
  }

  void BlinnPhongShaderProgram::setDiffuseTexture(const Texture2D* texture) const
  {
    if (texture)
    {
      mDiffuseTextureId = texture->getId();
      glUniform1i(mHasDiffuseTexture, true);
      glUniform1i(mDiffuseTextureLocation, DIFFUSE_TEXTURE_UNIT);
    }
    else
    {
      glUniform1i(mHasDiffuseTexture, false);
    }
  }

  void BlinnPhongShaderProgram::setProjection(const glm::mat4& projection) const
  {
    bind();
    glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
  }

  PointLight* BlinnPhongShaderProgram::addPointLight(
    const PointLightParams& params, const glm::vec3& lightSourcePos
  )
  {
    return mPointLights.addLight(params, lightSourcePos);
  }

  void BlinnPhongShaderProgram::removePointLight(unsigned int index)
  {
    mPointLights.removeLight(index);
  }

  void BlinnPhongShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mUVScale = getUniformLocation("uvScale");
    mAmbient = getUniformLocation("material.ambient");
    mDiffuse = getUniformLocation("material.diffuse");
    mSpecular = getUniformLocation("material.specular");
    mShininess = getUniformLocation("material.shininess");
    mDiffuseTextureLocation = getUniformLocation("diffuseTexture");
    mHasDiffuseTexture = getUniformLocation("hasDiffuseTexture");
  }
}  // namespace RenderSystem
