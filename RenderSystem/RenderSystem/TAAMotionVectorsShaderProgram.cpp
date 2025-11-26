#include "TAAMotionVectorsShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  TAAMotionVectorsShaderProgram::TAAMotionVectorsShaderProgram(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath),
      mModel(),
      mPrevView(),
      mCurrentView(),
      mProjection(),
      mJitteredProjection()
  {
    initUniformLocations();
  }

  void TAAMotionVectorsShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void TAAMotionVectorsShaderProgram::setPrevView(const glm::mat4& view)
  {
    invoke([this, &view]()
           { glUniformMatrix4fv(mPrevView, 1, false, glm::value_ptr(view)); });
  }

  void TAAMotionVectorsShaderProgram::setCurrentView(const glm::mat4& view)
  {
    invoke([this, &view]()
           { glUniformMatrix4fv(mCurrentView, 1, false, glm::value_ptr(view)); });
  }

  void TAAMotionVectorsShaderProgram::setProjection(const glm::mat4& projection)
  {
    invoke([this, &projection]()
           { glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection)); });
  }

  void TAAMotionVectorsShaderProgram::setJitteredProjection(const glm::mat4& projection)
  {
    invoke(
      [this, &projection]()
      { glUniformMatrix4fv(mJitteredProjection, 1, false, glm::value_ptr(projection)); }
    );
  }

  void TAAMotionVectorsShaderProgram::initUniformLocations()
  {
    mModel = glGetUniformLocation(mShaderProgram, "model");
    mPrevView = glGetUniformLocation(mShaderProgram, "prevView");
    mCurrentView = glGetUniformLocation(mShaderProgram, "currentView");
    mProjection = glGetUniformLocation(mShaderProgram, "projection");
    mJitteredProjection = glGetUniformLocation(mShaderProgram, "jitteredProjection");
  }
}  // namespace RenderSystem
