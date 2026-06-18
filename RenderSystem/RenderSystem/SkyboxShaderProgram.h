#pragma once

#include <filesystem>
#include <glm/glm.hpp>

#include "CameraListener.h"
#include "CubemapTexture.h"
#include "ShaderProgram.h"

using namespace std::filesystem;

namespace RenderSystem
{
  class Camera;
  class Viewport;

  class SkyboxShaderProgram : public ShaderProgram, public CameraListener
  {
   public:
    SkyboxShaderProgram(const path& vertexShader, const path& fragmentShader);

    void preRenderSetup() const override;
    void onCameraChanged(const Camera* camera) const override;

    void setProjection(const glm::mat4& projection) const;
    void setSkyboxCubemap(const CubemapTexture& texture) const;

   private:
    void initUniformLocations();

   private:
    mutable int mSkyboxId;
    int mSkyboxLocation;

    int mView;
    int mProjection;
  };
}
