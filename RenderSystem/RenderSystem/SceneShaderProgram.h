#pragma once

#include "AbstractShaderProgram.h"
#include "DepthTexture.h"
#include "DirectionalLight.h"
#include "GeometryCore/Typedefs.h"
#include "Material.h"
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
    void setMaterialParams(const Object3DMaterialParams& materialParams);
    void setDirectionalLightParams(const DirectionalLightParams& light);
    void setView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);
    void setDepthMap(const DepthTexture& texture) const;
    void setLightView(const glm::mat4& lightView);
    void setLightProjection(const glm::mat4& lightProjection);
    PointLight* addPointLight(
      const PointLightParams& params, const Point3D& lightSourcePos
    );
    void removePointLight(unsigned int index);

   private:
    void setDirLightSourcePos(const Point3D& lightSourcePos);
    void initUniformLocations();
    void setUp();

   private:
    DirectionalLight mDirectionalLight;
    PointLights mPointLights;
    Material mMaterial;
    int mModel;
    int mLightView;
    int mLightProjection;
    int mView;
    int mProjection;
    int mShadowBias;
    int mDepthMap;
  };
}  // namespace RenderSystem
