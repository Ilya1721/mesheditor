#pragma once

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "CameraListener.h"

using namespace std::filesystem;

namespace RenderSystem
{
  class TAADepthMapShaderProgram : public Object3DShaderProgram, public CameraListener
  {
   public:
    TAADepthMapShaderProgram(
      const path& vertexShaderPath, const path& fragmentShaderPath
    );

    void onCameraChanged(const Camera* camera) const override;
    void setModel(const glm::mat4& model) const override;
    void setMaterial(const Material& material) const override {};

    void setProjection(const glm::mat4& projection) const;

   private:
    void initUniformLocations();

   private:
    int mModel;
    int mView;
    int mProjection;
  };
}  // namespace RenderSystem
