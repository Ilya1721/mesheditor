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
      mPrevModel(),
      mCurrentModel(),
      mPrevView(),
      mCurrentView(),
      mPrevJitteredProjection(),
      mCurrentJitteredProjection()
  {
    initUniformLocations();
  }

  void TAAMotionVectorsShaderProgram::setPrevModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mPrevModel, 1, false, glm::value_ptr(model)); });
  }

  void TAAMotionVectorsShaderProgram::setCurrentModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mCurrentModel, 1, false, glm::value_ptr(model)); });
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

  void TAAMotionVectorsShaderProgram::setPrevJitteredProjection(
    const glm::mat4& projection
  )
  {
    invoke(
      [this, &projection]()
      {
        glUniformMatrix4fv(mPrevJitteredProjection, 1, false, glm::value_ptr(projection));
      }
    );
  }

  void TAAMotionVectorsShaderProgram::setCurrentJitteredProjection(
    const glm::mat4& projection
  )
  {
    invoke(
      [this, &projection]()
      {
        glUniformMatrix4fv(
          mCurrentJitteredProjection, 1, false, glm::value_ptr(projection)
        );
      }
    );
  }

  void TAAMotionVectorsShaderProgram::initUniformLocations()
  {
    mPrevModel = getUniformLocation("prevModel");
    mCurrentModel = getUniformLocation("currentModel");
    mPrevView = getUniformLocation("prevView");
    mCurrentView = getUniformLocation("currentView");
    mPrevJitteredProjection = getUniformLocation("prevJitteredProjection");
    mCurrentJitteredProjection = getUniformLocation("currentJitteredProjection");
  }
}  // namespace RenderSystem
