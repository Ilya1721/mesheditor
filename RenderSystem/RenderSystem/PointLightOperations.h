#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class Scene;

  class PointLightOperations : public Operation
  {
   public:
    PointLightOperations(Scene* scene);
    void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos) override;
    void onMouseScroll(double offset) override {};
    void onMouseClick() override;
    void onKeyPressed(int key) override;

   private:
    void addPointLight();
    void removePointLight();

   private:
    unsigned int mSelectedLightIdx;
    Scene* mScene;
  };
}  // namespace RenderSystem
