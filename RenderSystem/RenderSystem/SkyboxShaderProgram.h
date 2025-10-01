#pragma once

#include <filesystem>
#include <glm/glm.hpp>

#include "CubemapTexture.h"
#include "ShaderProgram.h"

using namespace std::filesystem;

namespace RenderSystem
{
  class SkyboxShaderProgram : public ShaderProgram
  {
  public:
    SkyboxShaderProgram(const path& vertexShader, const path& fragmentShader);

    void setView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);
    void setSkyboxCubemap(const CubemapTexture& texture) const;

  private:
    void initUniformLocations();

  private:
    int mView;
    int mProjection;
    int mSkybox;
  };
}
