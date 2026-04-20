#include "PointLightOperations.h"

#include <glfw/glfw3.h>

#include "Constants.h"
#include "Scene.h"

namespace RenderSystem
{
  PointLightOperations::PointLightOperations(Scene* scene)
    : mScene(scene), mSelectedLightIdx()
  {
  }

  void PointLightOperations::onMouseMove(
    const glm::vec2& startCursorPos, const glm::vec2& endCursorPos
  )
  {
  }

  void PointLightOperations::onMouseClick()
  {
  }

  void PointLightOperations::onKeyPressed(int key)
  {
    switch (key)
    {
      case GLFW_KEY_L:
        addPointLight();
        break;
      case GLFW_KEY_D:
        removePointLight();
        break;
      default:
        break;
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
