#include "DirectionalLight.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "LightParams.h"
#include "glad/glad.h"

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
    setParams(DIRECTIONAL_LIGHT_PARAMS);
  }

  int DirectionalLight::getUniformLocation(const char* name) const
  {
    return glGetUniformLocation(mShaderProgram, name);
  }

  void DirectionalLight::initUniformLocations()
  {
    mCameraPos = getUniformLocation("cameraPos");
    mLightSourcePos = getUniformLocation("dirLightPos");
    mAmbient = getUniformLocation("directionalLightParams.ambient");
    mDiffuse = getUniformLocation("directionalLightParams.diffuse");
    mSpecular = getUniformLocation("directionalLightParams.specular");
  }

  void DirectionalLight::setParams(const DirectionalLightParams& params)
  {
    setAmbient(glm::value_ptr(params.ambient));
    setDiffuse(glm::value_ptr(params.diffuse));
    setSpecular(glm::value_ptr(params.specular));
  }

  void DirectionalLight::setLightSourcePos(const float* lightSourcePos) const
  {
    glUniform3fv(mLightSourcePos, 1, lightSourcePos);
  }

  void DirectionalLight::setCameraPos(const float* cameraPos) const
  {
    glUniform3fv(mCameraPos, 1, cameraPos);
  }

  void DirectionalLight::setAmbient(const float* ambient) const
  {
    glUniform3fv(mAmbient, 1, ambient);
  }

  void DirectionalLight::setDiffuse(const float* diffuse) const
  {
    glUniform3fv(mDiffuse, 1, diffuse);
  }

  void DirectionalLight::setSpecular(const float* specular) const
  {
    glUniform3fv(mSpecular, 1, specular);
  }
}  // namespace RenderSystem
