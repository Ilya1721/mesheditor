#include "ScreenShaderProgram.h"

namespace RenderSystem
{
  ScreenShaderProgram::ScreenShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath), mScreenTexture()
  {
    initUniformLocations();
  }

  void ScreenShaderProgram::setTexture(const Texture2D& texture)
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mScreenTexture, 0); });
  }

  void ScreenShaderProgram::initUniformLocations()
  {
    mScreenTexture = getUniformLocation("screenTexture");
  }
}  // namespace RenderSystem
