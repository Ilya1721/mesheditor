#pragma once

#include "AbstractShaderProgram.h"
#include "CameraListener.h"
#include "DirectionalLight.h"
#include "GeometryCore/Typedefs.h"
#include "ImageTexture.h"
#include "PointLights.h"
#include "ShaderProgram.h"

using namespace GeometryCore;

namespace RenderSystem
{
  class BlinnPhongShaderProgram :
    public ShaderProgram,
    public AbstractShaderProgram,
    public CameraListener
  {
    friend class DirLightSource;

   public:
    BlinnPhongShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );
    void onCameraPosChanged(Camera* camera) override;
    void setModel(const glm::mat4& model) override;
    void setDirLightParams(const DirectionalLightParams& light);
    void setDirLightSourcePos(const Point3D& pos);
    void setProjection(const glm::mat4& projection);
    void setAmbient(const RGB& ambient);
    void setDiffuse(const RGB& diffuse);
    void setSpecular(const RGB& specular);
    void setShininess(float shininess);
    void setDiffuseTexture(ImageTexture* texturePtr);
    PointLight* addPointLight(
      const PointLightParams& params, const Point3D& lightSourcePos
    );
    void removePointLight(unsigned int index);
    void setUVScale(const glm::vec2& scale);

   private:
    void setCameraPos(const Point3D& cameraPos);
    void setView(const glm::mat4& view);
    void initUniformLocations();

   private:
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
    int mDiffuseTexture;
    int mHasDiffuseTexture;
  };
}  // namespace RenderSystem
