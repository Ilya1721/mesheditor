#include "Zoom.h"

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    Zoom::Zoom(Scene* scene)
        : Operation(scene)
    {}

    void Zoom::onMouseScroll(double offset)
    {
        const auto& viewport = mScene->getParentWindow()->getViewport();
        if (viewport->getProjectionType() == PROJECTION_TYPE::PERSPECTIVE)
        {
            mScene->getCamera().zoom(offset * Scene::getRootObject().getBBox().getHeight() * ZOOM_STEP_COEF);
        }
        else
        {
            viewport->zoom(-offset * ORTHO_ZOOM_STEP);
        }
    }
}