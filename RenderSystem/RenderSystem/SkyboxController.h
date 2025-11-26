#pragma once

#include <array>
#include <functional>

#include "CameraListener.h"
#include "CubemapTexture.h"
#include "SkyboxShaderProgram.h"
#include "ViewportListener.h"

namespace RenderSystem
{
  class SceneShaderProgram;

  class SkyboxController : public CameraListener, public ViewportListener
  {
   public:
    SkyboxController(
      const path& vertexShader,
      const path& fragmentShader,
      const std::array<path, 6>& cubemapTextures,
      SceneShaderProgram* sceneShaderProgram
    );

    void onCameraPosChanged(Camera* camera) override;
    void onViewportChanged(Viewport* viewport) override;

    void render(const std::function<void()>& renderFunc);

   private:
    void init();

   private:
    SkyboxShaderProgram mShaderProgram;
    CubemapTexture mCubemapTexture;
    SceneShaderProgram* mSceneShaderProgram;
  };
}  // namespace RenderSystem
