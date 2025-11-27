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
    [[maybe_unused]] const Point2D& startCursorPos,
    [[maybe_unused]] const Point2D& endCursorPos
  )
  {
    if (mEnabled)
    {
      auto intersectionData = mWindow->getCursorSceneIntersection();
      mScene->setHighlightedFacesData(
        {intersectionData.raySurfaceIntersection.surfaceIndices,
         intersectionData.intersectedObject}
      );
    }
  }

  void SurfaceHighlighter::onKeyPressed(int key)
  {
    if (key != GLFW_KEY_H) { return; }

    mEnabled = !mEnabled;

    if (!mEnabled) { mScene->setHighlightedFacesData({}); }
  }
}  // namespace RenderSystem