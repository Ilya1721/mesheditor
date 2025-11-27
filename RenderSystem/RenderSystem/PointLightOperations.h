#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class Scene;

  class PointLightOperations : public Operation
  {
   public:
    PointLightOperations(Scene* scene);
    void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override;
    void onMouseScroll(double offset) override {};
    void onMouseClick() override;
    void onKeyPressed(int key) override;

   private:
     void addPointLight();
     void removePointLight();

   private:
     unsigned int mSelectedLightIdx;
     Scene* mScene;
  };
}  // namespace RenderSystem
