#include "PointCloudShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"

namespace RenderSystem
{
  PointCloudShaderProgram::PointCloudShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void PointCloudShaderProgram::onCameraPosChanged(Camera* camera)
  {
    setView(camera->getViewMatrix());
  }

  void PointCloudShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void PointCloudShaderProgram::setProjection(const glm::mat4& projection)
  {
    invoke([this, &projection]()
           { glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection)); });
  }

  void PointCloudShaderProgram::setPointScale(float pointScale)
  {
    invoke([this, &pointScale]() { glUniform1f(mPointScale, pointScale); });
  }

  void PointCloudShaderProgram::setMinPointSize(float minPointSize)
  {
    invoke([this, &minPointSize]() { glUniform1f(mMinPointSize, minPointSize); });
  }

  void PointCloudShaderProgram::setMaxPointSize(float maxPointSize)
  {
    invoke([this, &maxPointSize]() { glUniform1f(mMaxPointSize, maxPointSize); });
  }

  void PointCloudShaderProgram::setLightPos(const glm::vec3& pos)
  {
    invoke([this, &pos]() { glUniform3fv(mLightPos, 1, glm::value_ptr(pos)); });
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

  void PointCloudShaderProgram::setView(const glm::mat4& view)
  {
    invoke([this, &view]() { glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view)); }
    );
  }
}