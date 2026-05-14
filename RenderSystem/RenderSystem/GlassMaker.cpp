#include "GlassMaker.h"

#include <glfw/glfw3.h>

#include "Scene.h"

namespace RenderSystem
{
  GlassMaker::GlassMaker(Scene* scene) : mScene(scene)
  {
  }

  void GlassMaker::onKeyPressed(int key)
  {
    auto pickedObject = mScene->getPickedObject();
    if (!pickedObject)
    {
      return;
    }

    if (key == GLFW_KEY_G)
    {
      mGlassMakerController.setGlassMaterial(pickedObject);
    }
    else if (key == GLFW_KEY_C)
    {
      mGlassMakerController.resetObjectMaterial(pickedObject);
    }
  }
}
