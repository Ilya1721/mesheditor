#pragma once

#include "AbstractShaderProgram.h"
#include "CameraListener.h"
#include "ShaderProgram.h"

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
    void setLightColor(const glm::vec3& lightColor);

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
  };
}  // namespace RenderSystem
