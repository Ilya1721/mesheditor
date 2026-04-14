#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class Window;
  class Scene;
  class Camera;
  class Viewport;

  class Mover : public Operation
  {
   public:
    Mover(Window* window, Scene* scene, Camera* camera, Viewport* viewport);

    void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos) override;
    void onMouseScroll(double offset) override {};
    void onMouseClick() override {};
    void onKeyPressed(int key) override;

   private:
    glm::mat4 getTranslationTransform(
      const glm::vec2& startCursorPos, const glm::vec2& endCursorPos
    ) const;

   private:
    Window* mWindow;
    Scene* mScene;
    Camera* mCamera;
    Viewport* mViewport;
  };
}  // namespace RenderSystem
