#include "PointLightOperations.h"

#include "Window.h"

namespace RenderSystem
{
  PointLightOperations::PointLightOperations(Window* window)
    : Operation(window), mSelectedLightIdx()
  {
  }

  void PointLightOperations::onMouseMove(
    const Point2D& startCursorPos, const Point2D& endCursorPos
  )
  {
  }

  void PointLightOperations::onMouseClick() {}

  void PointLightOperations::onKeyPressed(int key)
  {
    switch (key)
    {
      case GLFW_KEY_P: addPointLight(); break;
      case GLFW_KEY_D: removePointLight(); break;
      default: break;
    }
  }

  void PointLightOperations::addPointLight()
  {
    mWindow->addPointLight(POINT_LIGHT_PARAMS, mWindow->getDefaultPointLightSourcePos());
  }

  void PointLightOperations::removePointLight()
  {
    mWindow->removePointLight(mSelectedLightIdx);
  }
}  // namespace RenderSystem
