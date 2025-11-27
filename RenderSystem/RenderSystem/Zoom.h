#pragma once

#include "GeometryCore/Typedefs.h"
#include "Operation.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  class Scene;
  class Viewport;
  class Camera;

  class Zoom : public Operation
  {
   public:
    Zoom(Scene* scene, Viewport* viewport, Camera* camera);

    void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
      override {};
    void onMouseScroll(double offset) override;
    void onMouseClick() override {};
    void onKeyPressed(int key) {};

  private:
    Scene* mScene;
    Viewport* mViewport;
    Camera* mCamera;
  };
}  // namespace RenderSystem