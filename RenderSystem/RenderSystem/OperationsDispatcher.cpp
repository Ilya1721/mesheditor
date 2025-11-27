#include "OperationsDispatcher.h"

#include "Cloner.h"
#include "Mover.h"
#include "Orbit.h"
#include "Pan.h"
#include "Picker.h"
#include "PointLightOperations.h"
#include "SurfaceExtruder.h"
#include "SurfaceHighlighter.h"
#include "WireframeRenderer.h"
#include "Zoom.h"

namespace RenderSystem
{
  OperationsDispatcher::OperationsDispatcher(
    Window* window, Viewport* viewport, Scene* scene, Camera* camera
  )
    : mWindow(window), mViewport(viewport), mScene(scene), mCamera(camera)
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
    addOperation<SurfaceHighlighter>(mWindow, mScene);
    addOperation<SurfaceExtruder>(mWindow, mScene, mCamera);
    addOperation<WireframeRenderer>(mScene);
    addOperation<Orbit>(mWindow, mCamera);
    addOperation<Pan>(mWindow, mCamera, mViewport);
    addOperation<Zoom>(mScene, mViewport, mCamera);
    addOperation<Picker>(mWindow, mScene, mCamera);
    addOperation<Mover>(mWindow, mScene, mCamera, mViewport);
    addOperation<Cloner>(mScene);
    addOperation<PointLightOperations>(mScene);
  }
}  // namespace RenderSystem