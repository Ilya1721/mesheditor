#pragma once

#include "GeometryCore/Typedefs.h"
#include "Operation.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  class Scene;

  class WireframeRenderer : public Operation
  {
   public:
    WireframeRenderer(Scene* scene);

    void onMouseMove(const Point2D&, const Point2D&) override {};
    void onMouseScroll(double offset) override {};
    void onMouseClick() override {};
    void onKeyPressed(int key) override;

   private:
    Scene* mScene;
  };
}  // namespace RenderSystem
