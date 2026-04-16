#include "SurfaceHighlighter.h"

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
  SurfaceHighlighter::SurfaceHighlighter(Window* window, Scene* scene)
    : mScene(scene), mWindow(window)
  {
  }

  void SurfaceHighlighter::onMouseMove(
    [[maybe_unused]] const glm::vec2& startCursorPos,
    [[maybe_unused]] const glm::vec2& endCursorPos
  )
  {
    if (mEnabled)
    {
      auto cursorRay = mWindow->castCursorRay();
      auto intersectionData = mScene->getRayIntersection(cursorRay);
      mScene->setHighlightedFacesData(
        {intersectionData.raySurfaceIntersection.surfaceIndices,
         intersectionData.intersectedObject}
      );
    }
  }

  void SurfaceHighlighter::onKeyPressed(int key)
  {
    if (key != GLFW_KEY_H)
    {
      return;
    }

    mEnabled = !mEnabled;

    if (!mEnabled)
    {
      mScene->setHighlightedFacesData({});
    }
  }
}  // namespace RenderSystem