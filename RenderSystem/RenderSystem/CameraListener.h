#pragma once

namespace RenderSystem
{
  class Camera;

  class CameraListener
  {
   public:
    virtual ~CameraListener() = default;
    virtual void onCameraChanged(const Camera* camera) const = 0;
  };
}  // namespace RenderSystem
