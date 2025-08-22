#include "PointLight.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <format>
#include <glm/gtc/type_ptr.hpp>

#include "LightParams.h"
#include "glad.h"

namespace RenderSystem
{
  void PointLight::init(int shaderProgram, unsigned int index)
  {
    mShaderProgram = shaderProgram;
    mIndex = index;
    initUniformLocations();
  }

  void PointLight::initUniformLocations()
  {
    const auto position = std::format("pointLights[{}].pos", mIndex);
    const auto ambient = getParamsUniformName("ambient");
    const auto diffuse = getParamsUniformName("diffuse");
    const auto specular = getParamsUniformName("specular");
    const auto constant = getParamsUniformName("constant");
    const auto linear = getParamsUniformName("linear");
    const auto quadratic = getParamsUniformName("quadratic");

    mLightSourcePos = getUniformLocation(position.c_str());
    mAmbient = getUniformLocation(ambient.c_str());
    mDiffuse = getUniformLocation(diffuse.c_str());
    mSpecular = getUniformLocation(specular.c_str());
    mConstant = getUniformLocation(constant.c_str());
    mLinear = getUniformLocation(linear.c_str());
    mQuadratic = getUniformLocation(quadratic.c_str());
  }

  void PointLight::setLightSourcePos(const float* lightSourcePos) const
  {
    glUniform3fv(mLightSourcePos, 1, lightSourcePos);
  }

  std::string PointLight::getParamsUniformName(const std::string& paramName) const
  {
    return std::format("pointLights[{}].params.{}", mIndex, paramName);
  }

  void PointLight::setParams(const PointLightParams& light)
  {
    setAmbient(glm::value_ptr(light.ambient));
    setDiffuse(glm::value_ptr(light.diffuse));
    setSpecular(glm::value_ptr(light.specular));
    setConstant(light.constant);
    setLinear(light.linear);
    setQuadratic(light.quadratic);
  }

  int PointLight::getUniformLocation(const char* name) const
  {
    return glGetUniformLocation(mShaderProgram, name);
  }

  void PointLight::setAmbient(const float* ambient) const
  {
    glUniform3fv(mAmbient, 1, ambient);
  }

  void PointLight::setDiffuse(const float* diffuse) const
  {
    glUniform3fv(mDiffuse, 1, diffuse);
  }

  void PointLight::setSpecular(const float* specular) const
  {
    glUniform3fv(mSpecular, 1, specular);
  }

  void PointLight::setConstant(float constant) const { glUniform1f(mConstant, constant); }

  void PointLight::setLinear(float linear) const { glUniform1f(mLinear, linear); }

  void PointLight::setQuadratic(float quadratic) const
  {
    glUniform1f(mQuadratic, quadratic);
  }
}  // namespace RenderSystem