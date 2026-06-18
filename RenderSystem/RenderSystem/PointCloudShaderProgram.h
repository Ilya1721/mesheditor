#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "ShaderProgram.h"

namespace RenderSystem
{
  class PointCloudShaderProgram : public Object3DShaderProgram, public CameraListener
  {
   public:
    PointCloudShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void onCameraChanged(const Camera* camera) const override;
    void setModel(const glm::mat4& model) const override;
    void setMaterial(const Material& material) const override;

    void setProjection(const glm::mat4& projection) const;
    void setPointScale(float pointScale) const;
    void setMinPointSize(float minPointSize) const;
    void setMaxPointSize(float maxPointSize) const;
    void setLightPos(const glm::vec3& pos) const;

  private:
    void initUniformLocations();

   private:
     int mModel;
     int mView;
     int mProjection;
     int mPointScale;
     int mMinPointSize;
     int mMaxPointSize;
     int mLightPos;
  };
}
