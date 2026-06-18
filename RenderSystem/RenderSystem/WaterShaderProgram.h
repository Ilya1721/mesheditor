#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "CubemapTexture.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

namespace RenderSystem
{
  class WaterShaderProgram : public Object3DShaderProgram, public CameraListener
  {
   public:
    WaterShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void preRenderSetup() const override;
    void onCameraChanged(const Camera* camera) const override;
    void setModel(const glm::mat4& model) const override;
    void setMaterial(const Material& material) const override;

    void setProjection(const glm::mat4& projection) const;
    void setVTime(float time) const;
    void setFTime(float time) const;
    void setSkyboxCubemap(const CubemapTexture& texture) const;
    void setNormalMap(const Texture2D& texture) const;

   private:
    void initUniformLocations();
    void setCameraPos(const glm::vec3& cameraPos) const;
    void setView(const glm::mat4& view) const;
    void setWaves(const std::vector<Wave>& waves) const;

   private:
    mutable int mSkyboxId;
    int mSkyboxLocation;
    mutable int mNormalMapId;
    int mNormalMapLocation;

    int mModel;
    int mView;
    int mProjection;
    int mVTime;
    int mFTime;
    int mWaveCount;
    int mNormalMapMoves;
    int mNormalMapMoveCount;
    int mNormalStrength;
    int mDepthFalloff;
    int mFresnelPower;
    int mReflectionIntensity;
    int mCameraPos;
    int mDeepColor;
    int mShallowColor;
  };
}
