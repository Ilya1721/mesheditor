#pragma once

#include "MeshCore/AABBox.h"
#include "Operation.h"

namespace RenderSystem
{
  class Scene;

  class Cloner : public Operation
  {
   public:
    Cloner(Scene* scene);

    void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos)
      override {};
    void onMouseScroll(double offset) override {};
    void onMouseClick() override {};
    void onKeyPressed(int key) override;

   private:
    glm::mat4 getInitialTransform(const MeshCore::AABBox& bbox) const;

   private:
    Scene* mScene;
  };
}  // namespace RenderSystem
