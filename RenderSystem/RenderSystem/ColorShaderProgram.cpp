#include "ColorShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"
#include "Material.h"

namespace RenderSystem
{
  ColorShaderProgram::ColorShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : Object3DShaderProgram(vertexShaderPath, fragmentShaderPath),
      mModel(),
      mView(),
      mProjection(),
      mColor()
  {
    initUniformLocations();
  }

  void ColorShaderProgram::onCameraChanged(const Camera* camera) const
  {
    setView(camera->getViewMatrix());
  }

  void ColorShaderProgram::setModel(const glm::mat4& model) const
  {
    bind();
    glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model));
  }

  void ColorShaderProgram::setView(const glm::mat4& view) const
  {
    bind();
    glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view));
  }

  void ColorShaderProgram::setProjection(const glm::mat4& projection) const
  {
    bind();
    glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
  }

  void ColorShaderProgram::setMaterial(const Material& material) const
  {
    bind();
    const auto& colorMaterial = static_cast<const ColorMaterial&>(material);
    glUniform3fv(mColor, 1, glm::value_ptr(colorMaterial.color));
  }

  void ColorShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mColor = getUniformLocation("color");
  }
}
