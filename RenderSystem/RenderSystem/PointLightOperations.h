#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class PointLightOperations : public Operation
  {
   public:
    PointLightOperations(Window* window);
    void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override;
    void onMouseScroll(double offset) override {};
    void onMouseClick() override;
    void onKeyPressed(int key) override;

   private:
     void addPointLight();
     void removePointLight();

   private:
     unsigned int mSelectedLightIdx;
  };
}  // namespace RenderSystem
