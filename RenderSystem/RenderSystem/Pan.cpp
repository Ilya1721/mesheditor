#include "Pan.h"

#include "Window.h"

namespace RenderSystem
{
  Pan::Pan(Window* window) : Operation(window) {}

  void Pan::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
  {
    auto middleMouseButtonPressed =
      mWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE);
    auto shiftKeyPressed = mWindow->isKeyPressed(GLFW_KEY_LEFT_SHIFT);

    if (mWindow->isCameraMovementEnabled() && middleMouseButtonPressed && shiftKeyPressed)
    {
      mWindow->pan(
        mWindow->unProject(startCursorPos), mWindow->unProject(endCursorPos),
        mWindow->getProjectionType()
      );
    }
  }
}  // namespace RenderSystem