#include "TAADepthMapShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad/glad.h"

namespace RenderSystem
{
  TAADepthMapShaderProgram::TAADepthMapShaderProgram(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath),
      mModel(),
      mView(),
      mJitteredProjection()
  {
    initUniformLocations();
  }

  void TAADepthMapShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void TAADepthMapShaderProgram::setView(const glm::mat4& view)
  {
    invoke([this, &view]() { glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view)); }
    );
  }

  void TAADepthMapShaderProgram::setJitteredProjection(const glm::mat4& projection)
  {
    invoke(
      [this, &projection]()
      { glUniformMatrix4fv(mJitteredProjection, 1, false, glm::value_ptr(projection)); }
    );
  }

  void TAADepthMapShaderProgram::initUniformLocations()
  {
    mModel = glGetUniformLocation(mShaderProgram, "model");
    mView = glGetUniformLocation(mShaderProgram, "view");
    mJitteredProjection = glGetUniformLocation(mShaderProgram, "jitteredProjection");
  }
}  // namespace RenderSystem
