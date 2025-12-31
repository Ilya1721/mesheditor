#pragma once

#include <array>
#include <functional>

#include "CameraListener.h"
#include "CubemapTexture.h"
#include "SkyboxShaderProgram.h"
#include "ViewportListener.h"

namespace RenderSystem
{
  class SkyboxController : public CameraListener, public ViewportListener
  {
   public:
    SkyboxController(
      const path& vertexShader,
      const path& fragmentShader,
      const std::array<path, 6>& cubemapTextures
    );

    const CubemapTexture& getCubemapTexture();

    void onCameraPosChanged(Camera* camera) override;
    void onViewportChanged(Viewport* viewport) override;

    void render(const std::function<void()>& renderFunc);

   private:
    SkyboxShaderProgram mShaderProgram;
    CubemapTexture mCubemapTexture;
  };
}  // namespace RenderSystem
