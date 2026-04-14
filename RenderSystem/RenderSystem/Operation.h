#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
  class Operation
  {
   public:
    virtual ~Operation() = default;

    virtual void onMouseMove(
      const glm::vec2& startCursorPos, const glm::vec2& endCursorPos
    ) = 0;
    virtual void onMouseScroll(double offset) = 0;
    virtual void onMouseClick() = 0;
    virtual void onKeyPressed(int key) = 0;

   protected:
    bool mEnabled = false;
  };
}  // namespace RenderSystem
