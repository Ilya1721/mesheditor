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

    void setView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);
    void render();

   private:
    SkyboxShaderProgram mShaderProgram;
    CubemapTexture mCubemapTexture;
  };
}  // namespace RenderSystem
