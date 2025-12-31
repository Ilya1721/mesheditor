#pragma once

#include "AbstractShaderProgram.h"
#include "CameraListener.h"
#include "CubemapTexture.h"
#include "ShaderProgram.h"
#include "Typedefs.h"

namespace RenderSystem
{
  class GlassShaderProgram :
    public ShaderProgram,
    public AbstractShaderProgram,
    public CameraListener
  {
   public:
    GlassShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );
    void onCameraPosChanged(Camera* camera) override;
    void setModel(const glm::mat4& model) override;
    void setProjection(const glm::mat4& projection);
    void setRefractiveIndex(float refractiveIndex);
    void setReflectionStrength(float reflectionStrength);
    void setTransparency(float transparency) const;
    void setInterpolationFactor(float interpolationFactor);
    void setColor(const RGB& color);
    void setSkyboxCubemap(const CubemapTexture& texture);

  private:
    void initUniformLocations();

  private:
    int mModel;
    int mView;
    int mProjection;
    int mSkybox;
    int mRefractiveIndex;
    int mReflectionStrength;
    int mTransparency;
    int mInterpolationFactor;
    int mColor;
  };
}  // namespace RenderSystem
