#include "TAADepthMapShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"

namespace RenderSystem
{
  TAADepthMapShaderProgram::TAADepthMapShaderProgram(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : Object3DShaderProgram(vertexShaderPath, fragmentShaderPath),
      mModel(),
      mView(),
      mProjection()
  {
    initUniformLocations();
  }

  void TAADepthMapShaderProgram::onCameraChanged(const Camera* camera) const
  {
    bind();
    const auto& view = camera->getViewMatrix();
    glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view));
  }

  void TAADepthMapShaderProgram::setModel(const glm::mat4& model) const
  {
    bind();
    glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model));
  }

  void TAADepthMapShaderProgram::setProjection(const glm::mat4& projection) const
  {
    bind();
    glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
  }

  void TAADepthMapShaderProgram::initUniformLocations()
  {
    mModel = glGetUniformLocation(mShaderProgram, "model");
    mView = glGetUniformLocation(mShaderProgram, "view");
    mProjection = glGetUniformLocation(mShaderProgram, "projection");
  }
}  // namespace RenderSystem
