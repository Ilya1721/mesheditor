#include "TAAMotionVectorsShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"

namespace RenderSystem
{
  TAAMotionVectorsShaderProgram::TAAMotionVectorsShaderProgram(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath),
      mPrevModel(),
      mCurrModel(),
      mPrevView(),
      mCurrView(),
      mProjection(),
      mPrevViewMatrix(1.0f)
  {
    initUniformLocations();
  }

  void TAAMotionVectorsShaderProgram::setView(const glm::mat4& view) const
  {
    bind();
    glUniformMatrix4fv(mPrevView, 1, false, glm::value_ptr(mPrevViewMatrix));
    glUniformMatrix4fv(mCurrView, 1, false, glm::value_ptr(view));
    mPrevViewMatrix = view;
  }

  void TAAMotionVectorsShaderProgram::setPrevModel(const glm::mat4& model) const
  {
    bind();
    glUniformMatrix4fv(mPrevModel, 1, false, glm::value_ptr(model));
  }

  void TAAMotionVectorsShaderProgram::setCurrModel(const glm::mat4& model) const
  {
    bind();
    glUniformMatrix4fv(mCurrModel, 1, false, glm::value_ptr(model));
  }

  void TAAMotionVectorsShaderProgram::setProjection(const glm::mat4& projection) const
  {
    bind();
    glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
  }

  void TAAMotionVectorsShaderProgram::initUniformLocations()
  {
    mPrevModel = getUniformLocation("prevModel");
    mCurrModel = getUniformLocation("currModel");
    mPrevView = getUniformLocation("prevView");
    mCurrView = getUniformLocation("currView");
    mProjection = getUniformLocation("projection");
  }
}  // namespace RenderSystem
