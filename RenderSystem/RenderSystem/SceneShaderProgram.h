#pragma once

#include "AbstractShaderProgram.h"
#include "BlinnPhongMaterial.h"
#include "CameraListener.h"
#include "CubemapTexture.h"
#include "DepthTexture.h"
#include "DirectionalLight.h"
#include "GeometryCore/Typedefs.h"
#include "GlassMaterial.h"
#include "PointLights.h"
#include "ShaderProgram.h"

using namespace GeometryCore;

namespace RenderSystem
{
  class SceneShaderProgram :
    public ShaderProgram,
    public AbstractShaderProgram,
    public CameraListener
  {
    friend class DirLightSource;

   public:
    SceneShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void onCameraPosChanged(Camera* camera) override;

    void setModel(const glm::mat4& model) override;
    void setBlinnPhongMaterialParams(const BlinnPhongMaterialParamsExtended& params);
    void setGlassMaterialParams(const GlassMaterialParams& params);
    void setDirectionalLightParams(const DirectionalLightParams& light);
    void setJitteredProjection(const glm::mat4& projection);
    void setShadowMap(const DepthTexture& texture) const;
    void setLightView(const glm::mat4& lightView);
    void setLightProjection(const glm::mat4& lightProjection);
    void setShadowBias(float shadowBias);
    void setMaterialType(const MaterialType& materialType);
    void setSkyboxCubemap(const CubemapTexture& texture) const;
    PointLight* addPointLight(
      const PointLightParams& params, const Point3D& lightSourcePos
    );
    void removePointLight(unsigned int index);
    void setUVScale(const glm::vec2& scale);

   private:
    void setDirLightSourcePos(const Point3D& lightSourcePos);
    void setCameraPos(const Point3D& cameraPos);
    void setView(const glm::mat4& view);
    void initUniformLocations();

    void setUp();

   private:
    DirectionalLight mDirectionalLight;
    PointLights mPointLights;
    BlinnPhongMaterial mBlinnPhongMaterial;
    GlassMaterial mGlassMaterial;
    int mModel;
    int mLightView;
    int mLightProjection;
    int mView;
    int mJitteredProjection;
    int mShadowBias;
    int mShadowMap;
    int mMaterialType;
    int mSkybox;
    int mUVScale;
  };
}  // namespace RenderSystem
