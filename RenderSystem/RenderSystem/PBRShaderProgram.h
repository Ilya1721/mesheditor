#pragma once

#include "AbstractShaderProgram.h"
#include "CameraListener.h"
#include "ImageTexture.h"
#include "ShaderProgram.h"
#include "Typedefs.h"

namespace RenderSystem
{
  class PBRShaderProgram :
    public ShaderProgram,
    public AbstractShaderProgram,
    public CameraListener
  {
   public:
    PBRShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );
    void onCameraPosChanged(Camera* camera) override;
    void setModel(const glm::mat4& model);
    void setProjection(const glm::mat4& projection);
    void setUVScale(const glm::vec2& scale);
    void setLightPos(const glm::vec3& lightPos);
    void setLightColor(const RGB& lightColor);
    void setBaseColorTexture(const ImageTexture& texture);
    void setNormalTexture(const ImageTexture& texture);
    void setMetallicRoughnessTexture(const ImageTexture& texture);
    void setBaseColor(const RGB& color);
    void setMetallic(float metallic);
    void setRougness(float roughness);

   private:
    void setView(const glm::mat4& view);
    void setCameraPos(const glm::vec3& cameraPos);
    void initUniformLocations();

   private:
    int mModel;
    int mView;
    int mProjection;
    int mUVScale;
    int mCameraPos;
    int mLightPos;
    int mLightColor;
    int mHasBaseColorTexture;
    int mBaseColorTexture;
    int mHasNormalTexture;
    int mBaseColor;
    int mNormalTexture;
    int mHasMetallicRoughnessTexture;
    int mMetallic;
    int mRoughness;
    int mMetallicRoughnessTexture;
  };
}  // namespace RenderSystem
