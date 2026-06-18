#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

namespace RenderSystem
{
  class ShadowShaderProgram : public Object3DShaderProgram, public CameraListener
  {
   public:
    ShadowShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void preRenderSetup() const override;
    void onCameraChanged(const Camera* camera) const override;
    void setModel(const glm::mat4& model) const override;
    void setMaterial(const Material& material) const override {};

    void setShadowMap(const Texture2D& texture) const;
    void setLightView(const glm::mat4& lightView) const;
    void setLightProjection(const glm::mat4& lightProjection) const;
    void setShadowBias(float shadowBias) const;
    void setProjection(const glm::mat4& projection) const;
    void setUseSkinningTransform(bool useSkinningTransform) const;
    void setSkinningTransforms(const std::vector<glm::mat4>& skinningTransforms) const;

   private:
    void initUniformLocations();

   private:
    mutable int mShadowMapId;
    int mShadowMapLocation;

    int mModel;
    int mView;
    int mProjection;
    int mLightView;
    int mLightProjection;
    int mShadowBias;
    int mUseSkinningTransform;
    int mSkinningTransforms;
  };
}  // namespace RenderSystem
