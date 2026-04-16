#include "ParticlesGenerator.h"

#include <glfw/glfw3.h>

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
  ParticlesGenerator::ParticlesGenerator(Window* window, Scene* scene)
    : mWindow(window), mScene(scene)
  {
  }

  void ParticlesGenerator::onKeyPressed(int key)
  {
    if (key == GLFW_KEY_RIGHT)
    {
      mScene->moreParticles();
    }
    else if (key == GLFW_KEY_LEFT)
    {
      mScene->lessParticles();
    }
    else if (key == GLFW_KEY_P)
    {
      auto cursorRay = mWindow->castCursorRay();
      auto groundPlanePoint = mScene->getGroundPlaneIntersection(cursorRay);
      mScene->startGeneratingParticles(groundPlanePoint);
    }
    else if (key == GLFW_KEY_ESCAPE)
    {
      mScene->stopGeneratingParticles();
    }
  }
}
