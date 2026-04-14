#pragma once

#include "AbstractShaderProgram.h"
#include "CameraListener.h"
#include "DirectionalLight.h"
#include "ImageTexture.h"
#include "PointLights.h"
#include "ShaderProgram.h"

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
    void setDirLightSourcePos(const glm::vec3& pos);
    void setProjection(const glm::mat4& projection);
    void setAmbient(const glm::vec3& ambient);
    void setDiffuse(const glm::vec3& diffuse);
    void setSpecular(const glm::vec3& specular);
    void setShininess(float shininess);
    void setDiffuseTexture(ImageTexture* texturePtr);
    PointLight* addPointLight(
      const PointLightParams& params, const glm::vec3& lightSourcePos
    );
    void removePointLight(unsigned int index);
    void setUVScale(const glm::vec2& scale);

   private:
    void setCameraPos(const glm::vec3& cameraPos);
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
