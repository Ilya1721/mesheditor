#include "Pan.h"

#include "Camera.h"
#include "Viewport.h"
#include "Window.h"

namespace RenderSystem
{
  Pan::Pan(Window* window, Camera* camera, Viewport* viewport) : mWindow(window), mCamera(camera), mViewport(viewport) {}

  void Pan::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
  {
    auto middleMouseButtonPressed =
      mWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE);
    auto shiftKeyPressed = mWindow->isKeyPressed(GLFW_KEY_LEFT_SHIFT);

    if (mCamera->isMovementEnabled() && middleMouseButtonPressed && shiftKeyPressed)
    {
      mCamera->pan(
        mWindow->unProject(startCursorPos), mWindow->unProject(endCursorPos),
        mViewport->getProjectionType()
      );
    }
  }
}  // namespace RenderSystem