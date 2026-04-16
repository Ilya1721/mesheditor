#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class Scene;
  class Window;

  class ParticlesGenerator : public Operation
  {
   public:
    ParticlesGenerator(Window* window, Scene* scene);

    void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos)
      override {};
    void onMouseScroll(double offset) override {};
    void onMouseClick() override {};
    void onKeyPressed(int key) override;

   private:
    Window* mWindow;
    Scene* mScene;
  };
}
