#pragma once

#include "AbstractShaderProgram.h"
#include "CameraListener.h"
#include "DepthTexture.h"
#include "ShaderProgram.h"

namespace RenderSystem
{
  class ShadowShaderProgram :
    public ShaderProgram,
    public AbstractShaderProgram,
    public CameraListener
  {
   public:
    ShadowShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void onCameraPosChanged(Camera* camera) override;

    void setModel(const glm::mat4& model) override;
    void setShadowMap(const DepthTexture& texture) const;
    void setLightView(const glm::mat4& lightView);
    void setLightProjection(const glm::mat4& lightProjection);
    void setShadowBias(float shadowBias);
    void setProjection(const glm::mat4& projection);

   private:
    void initUniformLocations();

   private:
    int mModel;
    int mView;
    int mProjection;
    int mLightView;
    int mLightProjection;
    int mShadowBias;
    int mShadowMap;
  };
}  // namespace RenderSystem
