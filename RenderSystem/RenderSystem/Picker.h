#pragma once

#include "GeometryCore/Typedefs.h"
#include "Operation.h"

namespace RenderSystem
{
  class Object3D;

  class Window;
  class Scene;
  class Camera;

  class Picker : public Operation
  {
   public:
    Picker(Window* window, Scene* scene, Camera* camera);

    void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
      override {};
    void onMouseScroll(double offset) override {};
    void onMouseClick() override;
    void onKeyPressed(int key) override {};

  private:
    Window* mWindow;
    Scene* mScene;
    Camera* mCamera;
  };
}  // namespace RenderSystem
