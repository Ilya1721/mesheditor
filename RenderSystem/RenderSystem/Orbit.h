#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class Window;
  class Camera;

  class Orbit : public Operation
  {
   public:
    Orbit(Window* window, Camera* camera);

    void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos) override;
    void onMouseScroll(double offset) override {};
    void onMouseClick() override {};
    void onKeyPressed(int key) override {};

   private:
    Window* mWindow;
    Camera* mCamera;
  };
}  // namespace RenderSystem