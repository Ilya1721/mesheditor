#pragma once

#include "AbstractShaderProgram.h"
#include "DirectionalLight.h"
#include "GeometryCore/Typedefs.h"
#include "ShaderProgram.h"

using namespace GeometryCore;

namespace RenderSystem
{
  class SceneShaderProgram : public ShaderProgram, public AbstractShaderProgram
  {
    friend class LightSource;

   public:
    SceneShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void setModel(const glm::mat4& model) override;
    void setCameraPos(const Point3D& cameraPos);
    void setMaterial(const Material& material);
    void setDirectionalLightParams(const DirectionalLightParams& light);
    void setView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);
    void setDepthMap(int textureId);
    void setLightView(const glm::mat4& lightView);
    void setLightProjection(const glm::mat4& lightProjection);

   private:
    void setLightSourcePos(const Point3D& lightSourcePos);
    void initUniformLocations();
    void setUp();

   private:
    DirectionalLight mDirectionalLight;
    int mModel;
    int mLightView;
    int mLightProjection;
    int mView;
    int mProjection;
    int mShadowBias;
    int mDepthMap;
  };
}  // namespace RenderSystem
