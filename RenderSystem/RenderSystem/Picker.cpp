#include "Picker.h"

#include "Camera.h"
#include "Object3D.h"
#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
  Picker::Picker(Window* window, Scene* scene, Camera* camera)
    : mWindow(window), mScene(scene), mCamera(camera)
  {
  }

  void Picker::onMouseClick()
  {
    if (!mWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) ||
        !mCamera->isMovementEnabled())
    {
      return;
    }

    auto intersectionData = mWindow->getCursorSceneIntersection();
    if (!intersectionData.raySurfaceIntersection.surfaceIndices.empty())
    {
      auto pickedObject = intersectionData.intersectedObject;
      mScene->setPickedObject(pickedObject);
      mScene->highlightWholeObject(pickedObject);
    }
    else
    {
      mScene->setPickedObject(nullptr);
      mScene->highlightWholeObject(nullptr);
    }
  }
}  // namespace RenderSystem