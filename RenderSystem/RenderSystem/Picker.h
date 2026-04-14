#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class Object3D;

  class Window;
  class Scene;
  class Camera;

  class Picker : public Operation
  {
   public:
    Picker(Window* window, Scene* scene, Camera* camera);

    void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos)
      override {};
    void onMouseScroll(double offset) override {};
    void onMouseClick() override;
    void onKeyPressed(int key) override {};

   private:
    Window* mWindow;
    Scene* mScene;
    Camera* mCamera;
  };
}  // namespace RenderSystem
