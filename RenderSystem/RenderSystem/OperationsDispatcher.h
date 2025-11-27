#pragma once

#include <concepts>
#include <memory>
#include <vector>

#include "GeometryCore/Typedefs.h"
#include "Operation.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  class Window;
  class Viewport;
  class Scene;
  class Camera;

  class OperationsDispatcher
  {
   public:
    OperationsDispatcher(Window* window, Viewport* viewport, Scene* scene, Camera* camera);

    void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos);
    void onMouseScroll(double offset);
    void onMouseClick();
    void onKeyPressed(int key);

   private:
    void initOperations();

    template <typename OperationType, typename... OperationParams>
    void addOperation(const OperationParams&... params)
      requires std::derived_from<OperationType, Operation>
    {
      mOperations.emplace_back(std::make_unique<OperationType>(params...));
    }

   private:
    std::vector<std::unique_ptr<Operation>> mOperations;
    Window* mWindow;
    Viewport* mViewport;
    Scene* mScene;
    Camera* mCamera;
  };
}  // namespace RenderSystem
