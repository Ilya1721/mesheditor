#pragma once

#include <concepts>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

#include "GeometryCore/Typedefs.h"
#include "Operation.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  class OperationsDispatcher
  {
   public:
    OperationsDispatcher(Window* window);

    void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos);
    void onMouseScroll(double offset);
    void onMouseClick();
    void onKeyPressed(int key);

   private:
    void initOperations();

    template <typename T>
    void addOperation()
      requires std::derived_from<T, Operation>
    {
      mOperations.emplace_back(std::make_unique<T>(mWindow));
    }

   private:
    std::vector<std::unique_ptr<Operation>> mOperations;
    Window* mWindow;
  };
}  // namespace RenderSystem
