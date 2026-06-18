#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "ShaderProgram.h"

namespace RenderSystem
{
  class ColorShaderProgram :
    public Object3DShaderProgram,
    public CameraListener
  {
   public:
    ColorShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void onCameraChanged(const Camera* camera) const override;
    void setModel(const glm::mat4& model) const override;
    void setMaterial(const Material& material) const override;

    void setProjection(const glm::mat4& projection) const;

   private:
    void initUniformLocations();
    void setView(const glm::mat4& view) const;

   private:
    int mModel;
    int mView;
    int mProjection;
    int mColor;
  };
}
