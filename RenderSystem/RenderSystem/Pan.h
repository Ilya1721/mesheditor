#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class Window;
  class Camera;
  class Viewport;

  class Pan : public Operation
  {
   public:
    Pan(Window* window, Camera* camera, Viewport* viewport);

    void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos) override;
    void onMouseScroll(double offset) override {};
    void onMouseClick() override {};
    void onKeyPressed(int key) override {};

   private:
    Window* mWindow;
    Camera* mCamera;
    Viewport* mViewport;
  };
}  // namespace RenderSystem