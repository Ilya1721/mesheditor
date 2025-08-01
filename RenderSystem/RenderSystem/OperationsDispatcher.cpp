#include "OperationsDispatcher.h"

#include <glfw/glfw3.h>

#include "Cloner.h"
#include "Mover.h"
#include "Orbit.h"
#include "Pan.h"
#include "Picker.h"
#include "SurfaceExtruder.h"
#include "SurfaceHighlighter.h"
#include "WireframeRenderer.h"
#include "Zoom.h"

namespace RenderSystem
{
  OperationsDispatcher::OperationsDispatcher(Window* window) : mWindow(window)
  {
    initOperations();
  }

  void OperationsDispatcher::onMouseMove(
    const Point2D& startCursorPos, const Point2D& endCursorPos
  )
  {
    for (auto& operation : mOperations)
    {
      operation->onMouseMove(startCursorPos, endCursorPos);
    }
  }

  void OperationsDispatcher::onMouseScroll(double offset)
  {
    for (auto& operation : mOperations) { operation->onMouseScroll(offset); }
  }

  void OperationsDispatcher::onMouseClick()
  {
    for (auto& operation : mOperations) { operation->onMouseClick(); }
  }

  void OperationsDispatcher::onKeyPressed(int key)
  {
    for (auto& operation : mOperations) { operation->onKeyPressed(key); }
  }

  void OperationsDispatcher::initOperations()
  {
    addOperation<SurfaceHighlighter>();
    addOperation<SurfaceExtruder>();
    addOperation<WireframeRenderer>();
    addOperation<Orbit>();
    addOperation<Pan>();
    addOperation<Zoom>();
    addOperation<Picker>();
    addOperation<Mover>();
    addOperation<Cloner>();
  }
}  // namespace RenderSystem