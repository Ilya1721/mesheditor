#pragma once

#include "GlassMakerController.h"
#include "Operation.h"

namespace RenderSystem
{
  class Scene;

  class GlassMaker : public Operation
  {
   public:
    GlassMaker(Scene* scene);

    void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos)
      override {};
    void onMouseScroll(double offset) override {};
    void onMouseClick() override {};
    void onKeyPressed(int key) override;

   private:
    Scene* mScene;
    GlassMakerController mGlassMakerController;
  };
}  // namespace RenderSystem
