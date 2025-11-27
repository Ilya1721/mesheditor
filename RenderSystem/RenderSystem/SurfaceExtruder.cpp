#include "SurfaceExtruder.h"

#include "Camera.h"
#include "MeshCore/Face.h"
#include "MeshCore/Vertex.h"
#include "Object3D.h"
#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
  SurfaceExtruder::SurfaceExtruder(Window* window, Scene* scene, Camera* camera)
    : mWindow(window), mScene(scene), mCamera(camera), mSurfaceMovementEnabled(false)
  {
  }

  void SurfaceExtruder::onMouseMove(
    const Point2D& startCursorPos, const Point2D& endCursorPos
  )
  {
    if (!mEnabled || mIntersectionData.raySurfaceIntersection.surfaceIndices.empty() ||
        !mSurfaceMovementEnabled)
    {
      return;
    }

    auto& surface = mIntersectionData.raySurfaceIntersection.surface;
    auto startCursorPosInWorld = mWindow->unProject(startCursorPos);
    auto endCursorPosInWorld = mWindow->unProject(endCursorPos);
    auto surfaceNormal = glm::normalize(surface.normal);
    auto cursorMovement = glm::normalize(endCursorPosInWorld - startCursorPosInWorld);
    auto surfaceMovement = surfaceNormal * glm::dot(surfaceNormal, cursorMovement);
    std::unordered_set<MeshCore::UniqueVertex*> changedVertices;

    for (auto& face : surface.faces) { face->move(surfaceMovement, changedVertices); }

    mIntersectionData.intersectedObject->onMeshUpdated(changedVertices);
  }

  void SurfaceExtruder::onMouseClick()
  {
    if (mEnabled && mWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
      mIntersectionData = mWindow->getCursorSceneIntersection();
      toggleSurfaceMovement(
        !mIntersectionData.raySurfaceIntersection.surfaceIndices.empty()
      );
      highlightIntersectedSurface();
    }
  }

  void SurfaceExtruder::onKeyPressed(int key)
  {
    if (key != GLFW_KEY_E) { return; }

    mEnabled = !mEnabled;

    if (mEnabled) { mCamera->enableMovement(false); }
    else
    {
      mCamera->enableMovement(true);
      toggleSurfaceMovement();
    }
  }

  void SurfaceExtruder::highlightIntersectedSurface() const
  {
    if (mSurfaceMovementEnabled)
    {
      mScene->setHighlightedFacesData(
        {mIntersectionData.raySurfaceIntersection.surfaceIndices,
         mIntersectionData.intersectedObject}
      );
    }
  }

  void SurfaceExtruder::toggleSurfaceMovement(bool isSurfaceIntersected)
  {
    if (mSurfaceMovementEnabled) { mScene->setHighlightedFacesData({}); }

    mSurfaceMovementEnabled = isSurfaceIntersected && !mSurfaceMovementEnabled;
  }
}  // namespace RenderSystem