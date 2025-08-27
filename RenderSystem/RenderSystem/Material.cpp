#include "Material.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
#include <glm/gtc/type_ptr.hpp>

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

  void Material::setParams(const MeshCore::MaterialParams& material)
  {
    setAmbient(glm::value_ptr(material.ambient));
    setDiffuse(glm::value_ptr(material.diffuse));
    setSpecular(glm::value_ptr(material.specular));
    setShininess(material.shininess);
  }

  void Material::initUniformLocations()
  {
    mAmbient = getUniformLocation("material.ambient");
    mDiffuse = getUniformLocation("material.diffuse");
    mSpecular = getUniformLocation("material.specular");
    mShininess = getUniformLocation("material.shininess");
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
}  // namespace RenderSystem
