#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "CubemapTexture.h"
#include "ImageTexture.h"
#include "ShaderProgram.h"
#include "ViewportListener.h"
#include "Water.h"

namespace RenderSystem
{
  class WaterShaderProgram :
    public ShaderProgram,
    public CameraListener,
    public ViewportListener
  {
   public:
    WaterShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void onCameraPosChanged(Camera* camera) override;
    void onViewportChanged(Viewport* viewport) override;

    void setModel(const glm::mat4& model);
    void setVTime(float time);
    void setFTime(float time);
    void setWaves(const std::vector<Wave>& waves);
    void setNormalMapMoves(const std::vector<glm::vec2>& normalMapMoves);
    void setSkyboxCubemap(const CubemapTexture& texture) const;
    void setNormalMap(const ImageTexture& texture);
    void setNormalStrength(float normalStrength);
    void setDepthFalloff(float depthFalloff);
    void setFresnelPower(float fresnelPower);
    void setReflectionIntensity(float reflectionIntensity);
    void setDeepColor(const glm::vec3& deepColor);
    void setShallowColor(const glm::vec3& shallowColor);

   private:
    void initUniformLocations();
    void setCameraPos(const glm::vec3& cameraPos);
    void setView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);

   private:
    int mModel;
    int mView;
    int mProjection;
    int mVTime;
    int mFTime;
    int mWaveCount;
    int mSkybox;
    int mNormalMap;
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
