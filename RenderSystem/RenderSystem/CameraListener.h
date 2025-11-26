#pragma once

namespace RenderSystem
{
  class Camera;

  class CameraListener
  {
   public:
    virtual ~CameraListener() = default;

    virtual void onCameraPosChanged(Camera* camera) = 0;
  };
}  // namespace RenderSystem
