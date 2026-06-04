#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "ShaderProgram.h"

namespace RenderSystem
{
  class PointCloudShaderProgram : public ShaderProgram, public CameraListener
  {
   public:
    PointCloudShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void onCameraPosChanged(Camera* camera) override;
    void setModel(const glm::mat4& model);
    void setProjection(const glm::mat4& projection);
    void setPointScale(float pointScale);
    void setMinPointSize(float minPointSize);
    void setMaxPointSize(float maxPointSize);
    void setLightPos(const glm::vec3& pos);

  private:
    void initUniformLocations();
    void setView(const glm::mat4& view);

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
