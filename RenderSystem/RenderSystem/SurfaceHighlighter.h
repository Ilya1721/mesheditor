#pragma once

#include "GeometryCore/Typedefs.h"
#include "Operation.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  class Scene;
  class Window;

  class SurfaceHighlighter : public Operation
  {
   public:
    SurfaceHighlighter(Window* window, Scene* scene);

    void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override;
    void onMouseScroll(double offset) override {};
    void onMouseClick() override {};
    void onKeyPressed(int key) override;

   private:
    Scene* mScene;
    Window* mWindow;
  };
}  // namespace RenderSystem
