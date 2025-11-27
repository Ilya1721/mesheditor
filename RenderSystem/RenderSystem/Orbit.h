#pragma once

#include "GeometryCore/Typedefs.h"
#include "Operation.h"

namespace RenderSystem
{
  using namespace GeometryCore;
  
  class Window;
  class Camera;

  class Orbit : public Operation
  {
   public:
    Orbit(Window* window, Camera* camera);

    void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override;
    void onMouseScroll(double offset) override {};
    void onMouseClick() override {};
    void onKeyPressed(int key) override {};

  private:
    Window* mWindow;
    Camera* mCamera;
  };
}  // namespace RenderSystem