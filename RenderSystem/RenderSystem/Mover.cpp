#include "Mover.h"

#include <glm/gtx/transform.hpp>

#include "Camera.h"
#include "Object3D.h"
#include "Scene.h"
#include "Viewport.h"
#include "Window.h"

namespace RenderSystem
{
  Mover::Mover(Window* window, Scene* scene, Camera* camera, Viewport* viewport)
    : mWindow(window), mScene(scene), mCamera(camera), mViewport(viewport)
  {
  }

  void Mover::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
  {
    const auto& pickedObject = mScene->getPickedObject();
    if (mEnabled && pickedObject)
    {
      pickedObject->updateTransform(getTranslationTransform(startCursorPos, endCursorPos)
      );
    }
  }

  void Mover::onKeyPressed(int key)
  {
    if (key == GLFW_KEY_M) { mEnabled = !mEnabled; }
  }

  glm::mat4 Mover::getTranslationTransform(
    const Point2D& startCursorPos, const Point2D& endCursorPos
  ) const
  {
    const auto& startPosInWorldSpace = mWindow->unProject(startCursorPos);
    const auto& endPosInWorldSpace = mWindow->unProject(endCursorPos);

    if (mViewport->getProjectionType() == PROJECTION_TYPE::PERSPECTIVE)
    {
      return glm::translate(
        mCamera->projectToTargetPlane(endPosInWorldSpace) -
        mCamera->projectToTargetPlane(startPosInWorldSpace)
      );
    }
    else { return glm::translate(endPosInWorldSpace - startPosInWorldSpace); }
  }
}  // namespace RenderSystem