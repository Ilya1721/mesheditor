#include "Zoom.h"

#include "MeshCore/Object3D.h"

#include "GlobalRenderState.h"
#include "Viewport.h"
#include "Camera.h"
#include "Constants.h"

namespace
{
    using namespace RenderSystem;

    Camera* gCamera = &Camera::getInstance();
    Viewport* gViewport = &Viewport::getInstance();
    GlobalRenderState* gGlobalRenderState = &GlobalRenderState::getInstance();
}

namespace RenderSystem
{
    void Zoom::onMouseScroll(double offset)
    {
        if (gViewport->getProjectionType() == PROJECTION_TYPE::PERSPECTIVE)
        {
            gCamera->zoom(offset * gGlobalRenderState->getRootObject()->getBBox().getHeight() * ZOOM_STEP_COEF);
        }
        else
        {
            gViewport->zoom(-offset * ORTHO_ZOOM_STEP);
        }
    }
}