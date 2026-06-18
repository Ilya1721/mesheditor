#pragma once

#include "ShaderProgram.h"
#include "Texture2D.h"

namespace RenderSystem
{
  class ScreenShaderProgram : public ShaderProgram
  {
   public:
    ScreenShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void preRenderSetup() const override;
    void setScreenTexture(const Texture2D& texture);

   private:
    void initUniformLocations();

   private:
    mutable int mScreenTextureId;
    int mScreenTextureLocation;
  };
}  // namespace RenderSystem
