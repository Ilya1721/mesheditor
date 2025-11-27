#pragma once

#include "GeometryCore/Typedefs.h"
#include "Object3DIntersection.h"
#include "Operation.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  class Window;
  class Scene;
  class Camera;

  class SurfaceExtruder : public Operation
  {
   public:
    SurfaceExtruder(Window* window, Scene* scene, Camera* camera);

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
    Window* mWindow;
    Scene* mScene;
    Camera* mCamera;
  };
}  // namespace RenderSystem
