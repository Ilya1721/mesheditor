#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

namespace RenderSystem
{
  class ParticlesShaderProgram : public ShaderProgram, public CameraListener
  {
   public:
    ParticlesShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void preRenderSetup() const override;
    void onCameraChanged(const Camera* camera) const override;

    void setProjection(const glm::mat4& projection) const;
    void setFlipbookCols(int cols) const;
    void setFlipbookRows(int rows) const;
    void setFlipbookTexture(const Texture2D& texture) const;

   private:
    void initUniformLocations();

   private:
    mutable int mFlipbookTextureId;
    int mFlipbookTextureLocation;

    int mView;
    int mProjection;
    int mFlipbookCols;
    int mFlipbookRows;
    int mFlipbookTexture;
  };
}
