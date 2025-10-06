#pragma once

#include <array>
#include <functional>

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
    void render(const std::function<void()>& renderFunc);

   private:
    void init();

   private:
    SkyboxShaderProgram mShaderProgram;
    CubemapTexture mCubemapTexture;
  };
}  // namespace RenderSystem
