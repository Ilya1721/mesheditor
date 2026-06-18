#pragma once

#include "CameraListener.h"
#include "DirectionalLight.h"
#include "PointLights.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

namespace RenderSystem
{
  class BlinnPhongShaderProgram : public Object3DShaderProgram, public CameraListener
  {
    friend class DirLightSource;

   public:
    BlinnPhongShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void preRenderSetup() const override;
    void onCameraChanged(const Camera* camera) const override;
    void setModel(const glm::mat4& model) const override;
    void setMaterial(const Material& material) const override;

    void setProjection(const glm::mat4& projection) const;
    void setDirLightParams(const DirectionalLightParams& light) const;
    void setDirLightSourcePos(const glm::vec3& pos) const;
    PointLight* addPointLight(
      const PointLightParams& params, const glm::vec3& lightSourcePos
    );
    void removePointLight(unsigned int index);

   private:
    void setCameraPos(const glm::vec3& cameraPos) const;
    void setView(const glm::mat4& view) const;
    void setDiffuseTexture(const Texture2D* texture) const;
    void initUniformLocations();

   private:
    int mHasDiffuseTexture;
    mutable int mDiffuseTextureId;
    int mDiffuseTextureLocation;

    DirectionalLight mDirectionalLight;
    PointLights mPointLights;
    int mModel;
    int mView;
    int mProjection;
    int mUVScale;
    int mAmbient;
    int mDiffuse;
    int mSpecular;
    int mShininess;
  };
}  // namespace RenderSystem
