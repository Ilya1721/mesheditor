#include "GlassShaderProgram.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "glad/glad.h"

namespace RenderSystem
{
  GlassShaderProgram::GlassShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void GlassShaderProgram::onCameraPosChanged(Camera* camera)
  {
    invoke(
      [this, &camera]()
      { glUniformMatrix4fv(mView, 1, false, glm::value_ptr(camera->getViewMatrix())); }
    );
  }

  void GlassShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void GlassShaderProgram::setRefractiveIndex(float refractiveIndex)
  {
    invoke([this, &refractiveIndex]() { glUniform1f(mRefractiveIndex, refractiveIndex); }
    );
  }

  void GlassShaderProgram::setReflectionStrength(float reflectionStrength)
  {
    invoke([this, &reflectionStrength]()
           { glUniform1f(mReflectionStrength, reflectionStrength); });
  }

  void GlassShaderProgram::setTransparency(float transparency) const
  {
    invoke([this, &transparency]() { glUniform1f(mTransparency, transparency); });
  }

  void GlassShaderProgram::setInterpolationFactor(float interpolationFactor)
  {
    invoke([this, &interpolationFactor]()
           { glUniform1f(mInterpolationFactor, interpolationFactor); });
  }

  void GlassShaderProgram::setColor(const RGB& color)
  {
    invoke([this, &color]() { glUniform3fv(mColor, 1, glm::value_ptr(color)); });
  }

  void GlassShaderProgram::setSkyboxCubemap(const CubemapTexture& texture)
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mSkybox, 0); });
  }

  void GlassShaderProgram::setProjection(const glm::mat4& projection)
  {
    invoke([this, &projection]()
           { glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection)); });
  }

  void GlassShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mSkybox = getUniformLocation("skybox");
    mRefractiveIndex = getUniformLocation("material.refractiveIndex");
    mReflectionStrength = getUniformLocation("material.reflectionStrength");
    mTransparency = getUniformLocation("material.transparency");
    mInterpolationFactor = getUniformLocation("material.interpolationFactor");
    mColor = getUniformLocation("material.color");
  }
}  // namespace RenderSystem
