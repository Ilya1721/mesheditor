#include "DirectionalLight.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "LightParams.h"
#include "Material.h"
#include "glad.h"

namespace RenderSystem
{
  void DirectionalLight::init(int shaderProgram)
  {
    mShaderProgram = shaderProgram;
    initUniformLocations();
    setupSettings();
  }

  void DirectionalLight::setupSettings()
  {
    glClearColor(
      BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a
    );
    glEnable(GL_DEPTH_TEST);
    setLightParams(DIRECTIONAL_LIGHT_PARAMS);
    setMaterial(GOLD_MATERIAL);
  }

  int DirectionalLight::getUniformLocation(const char* name) const
  {
    return glGetUniformLocation(mShaderProgram, name);
  }

  void DirectionalLight::initUniformLocations()
  {
    mLightSourcePos = getUniformLocation("lightSourcePosInCameraSpace");
    mCameraPos = getUniformLocation("cameraPosInCameraSpace");
    mLightAmbient = getUniformLocation("directionalLight.ambient");
    mLightDiffuse = getUniformLocation("directionalLight.diffuse");
    mLightSpecular = getUniformLocation("directionalLight.specular");
    mMaterialAmbient = getUniformLocation("material.ambient");
    mMaterialDiffuse = getUniformLocation("material.diffuse");
    mMaterialSpecular = getUniformLocation("material.specular");
    mMaterialShininess = getUniformLocation("material.shininess");
  }

  void DirectionalLight::setMaterial(const Material& material)
  {
    setMaterialAmbient(glm::value_ptr(material.ambient));
    setMaterialDiffuse(glm::value_ptr(material.diffuse));
    setMaterialSpecular(glm::value_ptr(material.specular));
    setMaterialShininess(material.shininess);
  }

  void DirectionalLight::setLightParams(const DirectionalLightParams& params)
  {
    setLightAmbient(glm::value_ptr(params.ambient));
    setLightDiffuse(glm::value_ptr(params.diffuse));
    setLightSpecular(glm::value_ptr(params.specular));
  }

  void DirectionalLight::setLightSourcePos(const float* lightSourcePos) const
  {
    glUniform3fv(mLightSourcePos, 1, lightSourcePos);
  }

  void DirectionalLight::setCameraPos(const float* cameraPos) const
  {
    glUniform3fv(mCameraPos, 1, cameraPos);
  }

  void DirectionalLight::setLightAmbient(const float* ambient) const
  {
    glUniform3fv(mLightAmbient, 1, ambient);
  }

  void DirectionalLight::setLightDiffuse(const float* diffuse) const
  {
    glUniform3fv(mLightDiffuse, 1, diffuse);
  }

  void DirectionalLight::setLightSpecular(const float* specular) const
  {
    glUniform3fv(mLightSpecular, 1, specular);
  }

  void DirectionalLight::setMaterialAmbient(const float* ambient) const
  {
    glUniform3fv(mMaterialAmbient, 1, ambient);
  }

  void DirectionalLight::setMaterialDiffuse(const float* diffuse) const
  {
    glUniform3fv(mMaterialDiffuse, 1, diffuse);
  }

  void DirectionalLight::setMaterialSpecular(const float* specular) const
  {
    glUniform3fv(mMaterialSpecular, 1, specular);
  }

  void DirectionalLight::setMaterialShininess(float shininess) const
  {
    glUniform1f(mMaterialShininess, shininess);
  }
}  // namespace RenderSystem
