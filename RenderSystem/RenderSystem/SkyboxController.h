#pragma once

#include <array>

#include "CubemapTexture.h"
#include "SkyboxShaderProgram.h"

namespace RenderSystem
{
  class SkyboxController
  {
   public:
    SkyboxController(
      const path& vertexShader,
      const path& fragmentShader,
      const std::array<path, 6>& cubemapTextures
    );

   private:
    SkyboxShaderProgram mShaderProgram;
    CubemapTexture mCubemapTexture;
  };
}  // namespace RenderSystem
