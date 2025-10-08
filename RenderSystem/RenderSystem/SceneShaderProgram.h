#pragma once

#include "AbstractShaderProgram.h"
#include "BlinnPhongMaterial.h"
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
  class SceneShaderProgram : public ShaderProgram, public AbstractShaderProgram
  {
    friend class DirLightSource;

   public:
    SceneShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void setModel(const glm::mat4& model) override;
    void setCameraPos(const Point3D& cameraPos);
    void setBlinnPhongMaterialParams(const BlinnPhongMaterialParamsExtended& params);
    void setGlassMaterialParams(const GlassMaterialParams& params);
    void setDirectionalLightParams(const DirectionalLightParams& light);
    void setView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);
    void setDepthMap(const DepthTexture& texture) const;
    void setLightView(const glm::mat4& lightView);
    void setLightProjection(const glm::mat4& lightProjection);
    void setShadowBias(float shadowBias);
    void setMaterialType(const MaterialType& materialType);
    void setSkyboxCubemap(const CubemapTexture& texture) const;
    PointLight* addPointLight(
      const PointLightParams& params, const Point3D& lightSourcePos
    );
    void removePointLight(unsigned int index);

   private:
    void setDirLightSourcePos(const Point3D& lightSourcePos);
    void initUniformLocations();
    int getUniformLocation(const char* name) const;
    
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
    int mProjection;
    int mShadowBias;
    int mDepthMap;
    int mMaterialType;
    int mSkybox;
  };
}  // namespace RenderSystem
