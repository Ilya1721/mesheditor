#include "ParticlesShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"
#include "Viewport.h"

namespace RenderSystem
{

  ParticlesShaderProgram::ParticlesShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath),
      mView(),
      mProjection(),
      mFlipbookCols(),
      mFlipbookRows()
  {
  }

  void ParticlesShaderProgram::onCameraPosChanged(Camera* camera)
  {
    setView(camera->getViewMatrix());
  }

  void ParticlesShaderProgram::onViewportChanged(Viewport* viewport)
  {
    setProjection(viewport->getProjectionMatrix());
  }

  void ParticlesShaderProgram::initUniformLocations()
  {
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mFlipbookCols = getUniformLocation("flipbookCols");
    mFlipbookRows = getUniformLocation("flipbookRows");
    mFlipbookTexture = getUniformLocation("flipbookTexture");
  }

  void ParticlesShaderProgram::setView(const glm::mat4& view)
  {
    invoke([this, &view]() { glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view)); }
    );
  }

  void ParticlesShaderProgram::setProjection(const glm::mat4& projection)
  {
    invoke([this, &projection]()
           { glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection)); });
  }

  void ParticlesShaderProgram::setFlipbookCols(int cols)
  {
    invoke([this, cols]() { glUniform1i(mFlipbookCols, cols); });
  }

  void ParticlesShaderProgram::setFlipbookRows(int rows)
  {
    invoke([this, rows]() { glUniform1i(mFlipbookRows, rows); });
  }

  void ParticlesShaderProgram::setFlipbookTexture(const FlipbookTexture& texture)
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mFlipbookTexture, 0); });
  }
}