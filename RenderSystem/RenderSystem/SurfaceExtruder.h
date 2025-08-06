#pragma once

#include "GeometryCore/Typedefs.h"
#include "Object3DIntersection.h"
#include "Operation.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  class SurfaceExtruder : public Operation
  {
   public:
    SurfaceExtruder(Window* window);

    void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override;
    void onMouseScroll(double offset) override {};
    void onMouseClick() override;
    void onKeyPressed(int key) override;

   private:
    void toggleSurfaceMovement(bool isSurfaceIntersected = false);
    void highlightIntersectedSurface() const;

   private:
    Object3DIntersection mIntersectionData;
    bool mSurfaceMovementEnabled;
  };
}  // namespace RenderSystem
