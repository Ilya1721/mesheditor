#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "CubemapTexture.h"
#include "ShaderProgram.h"

namespace RenderSystem
{
  class GlassShaderProgram : public Object3DShaderProgram, public CameraListener
  {
   public:
    GlassShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void preRenderSetup() const override;
    void onCameraChanged(const Camera* camera) const override;
    void setModel(const glm::mat4& model) const override;
    void setMaterial(const Material& material) const override;

    void setProjection(const glm::mat4& projection) const;
    void setSkyboxCubemap(const CubemapTexture& texture) const;

   private:
    void initUniformLocations();

   private:
    mutable int mSkyboxTextureId;
    int mSkyboxTextureLocation;

    int mModel;
    int mView;
    int mProjection;
    int mRefractiveIndex;
    int mReflectionStrength;
    int mTransparency;
    int mInterpolationFactor;
    int mColor;
  };
}  // namespace RenderSystem
