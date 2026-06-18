#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

namespace RenderSystem
{
  class PBRShaderProgram : public Object3DShaderProgram, public CameraListener
  {
   public:
    PBRShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void preRenderSetup() const override;
    void onCameraChanged(const Camera* camera) const override;
    void setModel(const glm::mat4& model) const override;
    void setMaterial(const Material& material) const override;

    void setProjection(const glm::mat4& projection) const;
    void setLightPos(const glm::vec3& lightPos) const;
    void setLightColor(const glm::vec3& lightColor) const;
    void setUseSkinningTransform(bool useSkinningTransform) const;
    void setSkinningTransforms(const std::vector<glm::mat4>& skinningTransforms) const;

   private:
    void setView(const glm::mat4& view) const;
    void setCameraPos(const glm::vec3& cameraPos) const;
    void setBaseColorTexture(const Texture2D* texture) const;
    void setNormalTexture(const Texture2D* texture) const;
    void setMetallicRoughnessTexture(const Texture2D* texture) const;
    void initUniformLocations();

   private:
    int mHasBaseColorTexture;
    mutable int mBaseColorTextureId;
    int mBaseColorTextureLocation;

    int mHasNormalMapTexture;
    mutable int mNormalMapTextureId;
    int mNormalMapTextureLocation;

    int mHasMetallicRoughnessTexture;
    mutable int mMetallicRoughnessTextureId;
    int mMetallicRoughnessTextureLocation;

    int mModel;
    int mView;
    int mProjection;
    int mUVScale;
    int mCameraPos;
    int mLightPos;
    int mLightColor;
    int mBaseColor;
    int mMetallic;
    int mRoughness;
    int mUseSkinningTransform;
    int mSkinningTransforms;
  };
}  // namespace RenderSystem
