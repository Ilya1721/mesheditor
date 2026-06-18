#include "ScreenShaderProgram.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

namespace
{
  constexpr int SCREEN_TEXTURE_UNIT = 0;
}

namespace RenderSystem
{
  ScreenShaderProgram::ScreenShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void ScreenShaderProgram::preRenderSetup() const
  {
    glBindTextureUnit(SCREEN_TEXTURE_UNIT, mScreenTextureId);
  }

  void ScreenShaderProgram::setScreenTexture(const Texture2D& texture)
  {
    bind();
    mScreenTextureId = texture.getId();
    glUniform1i(mScreenTextureLocation, SCREEN_TEXTURE_UNIT);
  }

  void ScreenShaderProgram::initUniformLocations()
  {
    mScreenTextureLocation = getUniformLocation("screenTexture");
  }
}  // namespace RenderSystem
