#include "Material.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

namespace RenderSystem
{
  void Material::init(int shaderProgram)
  {
    mShaderProgram = shaderProgram;
    initUniformLocations();
  }

  int Material::getUniformLocation(const char* name) const
  {
    return glGetUniformLocation(mShaderProgram, name);
  }

  void Material::setParams(const Object3DMaterialParams& materialParams)
  {
    setAmbient(glm::value_ptr(materialParams.ambient));
    setDiffuse(glm::value_ptr(materialParams.diffuse));
    setSpecular(glm::value_ptr(materialParams.specular));
    setShininess(materialParams.shininess);
    setDiffuseTexture(materialParams.diffuseTexture);
  }

  void Material::initUniformLocations()
  {
    mAmbient = getUniformLocation("material.ambient");
    mDiffuse = getUniformLocation("material.diffuse");
    mSpecular = getUniformLocation("material.specular");
    mShininess = getUniformLocation("material.shininess");
    mDiffuseTexture = getUniformLocation("diffuseTexture");
    mHasDiffuseTexture = getUniformLocation("material.hasDiffuseTexture");
  }

  void Material::setAmbient(const float* ambient) const
  {
    glUniform3fv(mAmbient, 1, ambient);
  }

  void Material::setDiffuse(const float* diffuse) const
  {
    glUniform3fv(mDiffuse, 1, diffuse);
  }

  void Material::setSpecular(const float* specular) const
  {
    glUniform3fv(mSpecular, 1, specular);
  }

  void Material::setShininess(float shininess) const
  {
    glUniform1f(mShininess, shininess);
  }

  void Material::setDiffuseTexture(const ImageTexture& texture) const
  {
    if (texture.isEmpty()) { glUniform1i(mHasDiffuseTexture, false); }
    else
    {
      glUniform1i(mHasDiffuseTexture, true);
      texture.passToFragmentShader(mDiffuseTexture, texture.getTextureSlot());
    }
  }
}  // namespace RenderSystem
