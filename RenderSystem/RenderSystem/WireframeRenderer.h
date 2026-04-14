#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class Scene;

  class WireframeRenderer : public Operation
  {
   public:
    WireframeRenderer(Scene* scene);

    void onMouseMove(const glm::vec2&, const glm::vec2&) override {};
    void onMouseScroll(double offset) override {};
    void onMouseClick() override {};
    void onKeyPressed(int key) override;

   private:
    Scene* mScene;
  };
}  // namespace RenderSystem
