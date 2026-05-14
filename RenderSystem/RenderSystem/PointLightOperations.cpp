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

  void PointLightOperations::onKeyPressed(int key)
  {
    if (key == GLFW_KEY_L)
    {
      mScene->addPointLight(POINT_LIGHT_PARAMS, mScene->getDefaultPointLightSourcePos());
    }
    else if (key == GLFW_KEY_D)
    {
      mScene->removePointLight(mSelectedLightIdx);
    }
  }
}  // namespace RenderSystem
