#pragma once

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

    void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos) override;
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
