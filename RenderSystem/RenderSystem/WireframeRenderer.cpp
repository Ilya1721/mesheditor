#include "WireframeRenderer.h"

#include <glfw/glfw3.h>

#include "Scene.h"

namespace RenderSystem
{
  WireframeRenderer::WireframeRenderer(Scene* scene) : mScene(scene)
  {
  }

  void WireframeRenderer::onKeyPressed(int key)
  {
    if (key == GLFW_KEY_W)
    {
      mScene->toggleWireframe();
    }
  }
}  // namespace RenderSystem