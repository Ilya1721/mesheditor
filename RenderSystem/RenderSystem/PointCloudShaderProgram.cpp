#include "PointCloudShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"
#include "Material.h"

namespace RenderSystem
{
  PointCloudShaderProgram::PointCloudShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : Object3DShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void PointCloudShaderProgram::onCameraChanged(const Camera* camera) const
  {
    bind();
    const auto& view = camera->getViewMatrix();
    glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view));
  }

  void PointCloudShaderProgram::setModel(const glm::mat4& model) const
  {
    bind();
    glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model));
  }

  void PointCloudShaderProgram::setMaterial(const Material& material) const
  {
  }

  void PointCloudShaderProgram::setProjection(const glm::mat4& projection) const
  {
    bind();
    glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
  }

  void PointCloudShaderProgram::setPointScale(float pointScale) const
  {
    bind();
    glUniform1f(mPointScale, pointScale);
  }

  void PointCloudShaderProgram::setMinPointSize(float minPointSize) const
  {
    bind();
    glUniform1f(mMinPointSize, minPointSize);
  }

  void PointCloudShaderProgram::setMaxPointSize(float maxPointSize) const
  {
    bind();
    glUniform1f(mMaxPointSize, maxPointSize);
  }

  void PointCloudShaderProgram::setLightPos(const glm::vec3& pos) const
  {
    bind();
    glUniform3fv(mLightPos, 1, glm::value_ptr(pos));
  }

  void PointCloudShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mPointScale = getUniformLocation("pointScale");
    mMinPointSize = getUniformLocation("minPointSize");
    mMaxPointSize = getUniformLocation("maxPointSize");
    mLightPos = getUniformLocation("lightPos");
  }
}