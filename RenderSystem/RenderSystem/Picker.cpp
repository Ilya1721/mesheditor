#include "Picker.h"

#include "MeshCore/Intersection.h"
#include "MeshCore/Object3D.h"

#include "Window.h"
#include "GlobalRenderState.h"
#include "Camera.h"

namespace
{
    using namespace RenderSystem;

    Window* gWindow = &Window::getInstance();
    Camera* gCamera = &Camera::getInstance();
    GlobalRenderState* gGlobalRenderState = &GlobalRenderState::getInstance();
}

namespace RenderSystem
{
    void Picker::onMouseClick()
    {
        if (!gWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) || !gCamera->isMovementEnabled())
        {
            return;
        }

        auto intersection = gGlobalRenderState->getRootObject()->findIntersection(gWindow->castCursorRay(), true);
        if (!intersection.surfaceIndices.empty())
        {
            auto pickedObject = intersection.surface.getParentObject();
            gGlobalRenderState->setPickedObject(pickedObject);
            gGlobalRenderState->highlightWholeObject(pickedObject);
        }
        else
        {
            gGlobalRenderState->setPickedObject(nullptr);
            gGlobalRenderState->highlightWholeObject(nullptr);
        }
    }
}