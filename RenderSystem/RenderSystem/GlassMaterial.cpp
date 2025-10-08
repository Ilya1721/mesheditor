#include "GlassMaterial.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

namespace RenderSystem
{
  void GlassMaterial::init(int shaderProgram)
  {
    mShaderProgram = shaderProgram;
    initUniformLocations();
  }

  void GlassMaterial::setParams(const GlassMaterialParams& params)
  {
    setRefractiveIndex(params.refractiveIndex);
    setReflectionStrength(params.reflectionStrength);
    setTransparency(params.transparency);
    setInterpolationFactor(params.interpolationFactor);
    setColor(glm::value_ptr(params.color));
  }

  void GlassMaterial::setRefractiveIndex(float refractiveIndex) const
  {
    glUniform1f(mRefractiveIndex, refractiveIndex);
  }

  void GlassMaterial::setReflectionStrength(float reflectionStrength) const
  {
    glUniform1f(mReflectionStrength, reflectionStrength);
  }

  void GlassMaterial::setTransparency(float transparency) const
  {
    glUniform1f(mTransparency, transparency);
  }

  void GlassMaterial::setInterpolationFactor(float interpolationFactor) const
  {
    glUniform1f(mInterpolationFactor, interpolationFactor);
  }

  void GlassMaterial::setColor(const float* color) const
  {
    glUniform3fv(mColor, 1, color);
  }

  void GlassMaterial::initUniformLocations()
  {
    mRefractiveIndex = getUniformLocation("material.glass.refractiveIndex");
    mReflectionStrength = getUniformLocation("material.glass.reflectionStrength");
    mTransparency = getUniformLocation("material.glass.transparency");
    mInterpolationFactor = getUniformLocation("material.glass.interpolationFactor");
    mColor = getUniformLocation("material.glass.color");
  }

  int GlassMaterial::getUniformLocation(const char* name) const
  {
    return glGetUniformLocation(mShaderProgram, name);
  }
}  // namespace RenderSystem
