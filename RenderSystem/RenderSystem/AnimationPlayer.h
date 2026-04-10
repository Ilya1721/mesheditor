#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class Scene;

  class AnimationPlayer : public Operation
  {
   public:
    AnimationPlayer(Scene* scene);

    void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
      override {};
    void onMouseScroll(double offset) override {};
    void onMouseClick() override {};
    void onKeyPressed(int key) override;

   private:
    Scene* mScene;
  };
}
