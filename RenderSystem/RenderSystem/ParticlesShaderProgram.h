#pragma once

#include <glm/glm.hpp>

#include "CameraListener.h"
#include "FlipbookTexture.h"
#include "ShaderProgram.h"
#include "ViewportListener.h"

namespace RenderSystem
{
  class ParticlesShaderProgram :
    public ShaderProgram,
    public CameraListener,
    public ViewportListener
  {
   public:
    ParticlesShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void onCameraPosChanged(Camera* camera) override;
    void onViewportChanged(Viewport* viewport) override;

    void setProjection(const glm::mat4& projection);
    void setFlipbookCols(int cols);
    void setFlipbookRows(int rows);
    void setFlipbookTexture(const FlipbookTexture& texture);

   private:
    void initUniformLocations();
    void setView(const glm::mat4& view);

   private:
    int mView;
    int mProjection;
    int mFlipbookCols;
    int mFlipbookRows;
    int mFlipbookTexture;
  };
}
