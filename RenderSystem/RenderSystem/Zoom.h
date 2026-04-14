#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class Scene;
  class Viewport;
  class Camera;

  class Zoom : public Operation
  {
   public:
    Zoom(Scene* scene, Viewport* viewport, Camera* camera);

    void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos)
      override {};
    void onMouseScroll(double offset) override;
    void onMouseClick() override {};
    void onKeyPressed(int key) {};

   private:
    Scene* mScene;
    Viewport* mViewport;
    Camera* mCamera;
  };
}  // namespace RenderSystem