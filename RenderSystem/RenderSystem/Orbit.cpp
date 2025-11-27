#include "Orbit.h"

#include "Camera.h"
#include "Window.h"

namespace RenderSystem
{
  Orbit::Orbit(Window* window, Camera* camera) : mWindow(window), mCamera(camera) {}

  void Orbit::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
  {
    auto middleMouseButtonPressed =
      mWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE);
    auto shiftKeyPressed = mWindow->isKeyPressed(GLFW_KEY_LEFT_SHIFT);

    if (mCamera->isMovementEnabled() && middleMouseButtonPressed &&
        !shiftKeyPressed)
    {
      const auto offsetX = endCursorPos.x - startCursorPos.x;
      const auto offsetY = startCursorPos.y - endCursorPos.y;
      mCamera->smoothOrbit(offsetX, offsetY);
    }
  }
}  // namespace RenderSystem