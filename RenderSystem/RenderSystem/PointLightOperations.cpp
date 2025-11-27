#include "PointLightOperations.h"

#include "Constants.h"
#include "Scene.h"
#include <glfw/glfw3.h>

namespace RenderSystem
{
  PointLightOperations::PointLightOperations(Scene* scene)
    : mScene(scene), mSelectedLightIdx()
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
    mScene->addPointLight(POINT_LIGHT_PARAMS, mScene->getDefaultPointLightSourcePos());
  }

  void PointLightOperations::removePointLight()
  {
    mScene->removePointLight(mSelectedLightIdx);
  }
}  // namespace RenderSystem
