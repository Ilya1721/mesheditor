#include "ParticlesShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"
#include "Viewport.h"

namespace
{
  constexpr int FLIPBOOK_TEXTURE_UNIT = 0;
}

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
      mFlipbookRows(),
      mFlipbookTexture()
  {
    initUniformLocations();
  }

  void ParticlesShaderProgram::preRenderSetup() const
  {
    glBindTextureUnit(FLIPBOOK_TEXTURE_UNIT, mFlipbookTextureId);
  }

  void ParticlesShaderProgram::onCameraChanged(const Camera* camera) const
  {
    bind();
    const auto& view = camera->getViewMatrix();
    glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view));
  }

  void ParticlesShaderProgram::initUniformLocations()
  {
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mFlipbookCols = getUniformLocation("flipbookCols");
    mFlipbookRows = getUniformLocation("flipbookRows");
    mFlipbookTexture = getUniformLocation("flipbookTexture");
  }

  void ParticlesShaderProgram::setProjection(const glm::mat4& projection) const
  {
    bind();
    glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
  }

  void ParticlesShaderProgram::setFlipbookCols(int cols) const
  {
    bind();
    glUniform1i(mFlipbookCols, cols);
  }

  void ParticlesShaderProgram::setFlipbookRows(int rows) const
  {
    bind();
    glUniform1i(mFlipbookRows, rows);
  }

  void ParticlesShaderProgram::setFlipbookTexture(const Texture2D& texture) const
  {
    bind();
    mFlipbookTextureId = texture.getId();
    glUniform1i(mFlipbookTextureLocation, FLIPBOOK_TEXTURE_UNIT);
  }
}