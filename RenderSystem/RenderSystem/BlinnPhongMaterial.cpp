#include "BlinnPhongMaterial.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "glad/glad.h"

namespace RenderSystem
{
  void BlinnPhongMaterial::init(int shaderProgram)
  {
    mShaderProgram = shaderProgram;
    initUniformLocations();
  }

  int BlinnPhongMaterial::getUniformLocation(const char* name) const
  {
    return glGetUniformLocation(mShaderProgram, name);
  }

  void BlinnPhongMaterial::setParams(const BlinnPhongMaterialParamsExtended& params)
  {
    setAmbient(glm::value_ptr(params.ambient));
    setDiffuse(glm::value_ptr(params.diffuse));
    setSpecular(glm::value_ptr(params.specular));
    setShininess(params.shininess);
    setDiffuseTexture(params.diffuseTexture);
  }

  void BlinnPhongMaterial::initUniformLocations()
  {
    mAmbient = getUniformLocation("material.blinnPhong.ambient");
    mDiffuse = getUniformLocation("material.blinnPhong.diffuse");
    mSpecular = getUniformLocation("material.blinnPhong.specular");
    mShininess = getUniformLocation("material.blinnPhong.shininess");
    mDiffuseTexture = getUniformLocation("diffuseTexture");
    mHasDiffuseTexture = getUniformLocation("material.blinnPhong.hasDiffuseTexture");
  }

  void BlinnPhongMaterial::setAmbient(const float* ambient) const
  {
    glUniform3fv(mAmbient, 1, ambient);
  }

  void BlinnPhongMaterial::setDiffuse(const float* diffuse) const
  {
    glUniform3fv(mDiffuse, 1, diffuse);
  }

  void BlinnPhongMaterial::setSpecular(const float* specular) const
  {
    glUniform3fv(mSpecular, 1, specular);
  }

  void BlinnPhongMaterial::setShininess(float shininess) const
  {
    glUniform1f(mShininess, shininess);
  }

  void BlinnPhongMaterial::setDiffuseTexture(const ImageTexture& texture) const
  {
    if (texture.isEmpty()) { glUniform1i(mHasDiffuseTexture, false); }
    else
    {
      glUniform1i(mHasDiffuseTexture, true);
      texture.passToFragmentShader(mDiffuseTexture, DIFFUSE_TEXTURE_SLOT);
    }
  }
}  // namespace RenderSystem
