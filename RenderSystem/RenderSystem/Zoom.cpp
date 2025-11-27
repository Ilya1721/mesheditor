#include "Zoom.h"

#include "Camera.h"
#include "Constants.h"
#include "Scene.h"
#include "Viewport.h"

namespace RenderSystem
{
  Zoom::Zoom(Scene* scene, Viewport* viewport, Camera* camera)
    : mScene(scene), mViewport(viewport), mCamera(camera)
  {
  }

  void Zoom::onMouseScroll(double offset)
  {
    if (mViewport->getProjectionType() == PROJECTION_TYPE::PERSPECTIVE)
    {
      mCamera->zoom(
        offset * mScene->getRootObject().getBBox().getHeight() * ZOOM_STEP_COEF
      );
    }
    else { mViewport->zoom(-offset * ORTHO_ZOOM_STEP); }
  }
}  // namespace RenderSystem