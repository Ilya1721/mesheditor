#include "WaterGenerator.h"

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
  WaterGenerator::WaterGenerator(Window* window, Scene* scene)
    : mWindow(window), mScene(scene)
  {
  }

  void WaterGenerator::onKeyPressed(int key)
  {
    if (key == GLFW_KEY_W)
    {
      auto cursorRay = mWindow->castCursorRay();
      auto groundPlanePoint = mScene->getGroundPlaneIntersection(cursorRay, 1.2f);
      mScene->startGeneratingWater(groundPlanePoint);
    }
    else if (key == GLFW_KEY_ESCAPE)
    {
      mScene->stopGeneratingWater();
    }
  }
}
