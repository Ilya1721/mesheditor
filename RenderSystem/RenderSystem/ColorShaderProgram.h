#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "ShaderProgram.h"

namespace RenderSystem
{
  class ColorShaderProgram : public ShaderProgram, public CameraListener
  {
   public:
    ColorShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );
    void onCameraPosChanged(Camera* camera) override;
    void setModel(const glm::mat4& model);
    void setProjection(const glm::mat4& projection);
    void setColor(const glm::vec3& color);

   private:
    void initUniformLocations();
    void setView(const glm::mat4& view);

   private:
    int mModel;
    int mView;
    int mProjection;
    int mColor;
  };
}
