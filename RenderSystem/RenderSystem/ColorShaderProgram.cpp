#include "ColorShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"

namespace RenderSystem
{
  ColorShaderProgram::ColorShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath),
      mModel(),
      mView(),
      mProjection(),
      mColor()
  {
    initUniformLocations();
  }

  void ColorShaderProgram::onCameraPosChanged(Camera* camera)
  {
    setView(camera->getViewMatrix());
  }

  void ColorShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void ColorShaderProgram::setView(const glm::mat4& view)
  {
    invoke([this, &view]() { glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view)); }
    );
  }

  void ColorShaderProgram::setProjection(const glm::mat4& projection)
  {
    invoke([this, &projection]()
           { glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection)); });
  }

  void ColorShaderProgram::setColor(const glm::vec3& color)
  {
    invoke([this, &color]() { glUniform3fv(mColor, 1, glm::value_ptr(color)); });
  }

  void ColorShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mColor = getUniformLocation("color");
  }
}
