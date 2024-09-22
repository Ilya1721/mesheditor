#include "Picker.h"

#include "MeshCore/Intersection.h"
#include "MeshCore/Object3D.h"

#include "Window.h"
#include "GlobalRenderState.h"

namespace RenderSystem
{
    Picker::Picker(Window* window) :
        Operation(window)
    {}

    void Picker::onMouseClick()
    {
        if (!mWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) ||
            !mWindow->isCameraMovementEnabled())
        {
            return;
        }

        auto intersection = mWindow->getClosestIntersection();
        if (!intersection.surfaceIndices.empty())
        {
            auto pickedObject = intersection.surface.getParentObject();
            mWindow->setPickedObject(pickedObject);
            GlobalRenderState::highlightWholeObject(pickedObject);
        }
        else
        {
            mWindow->setPickedObject(nullptr);
            GlobalRenderState::highlightWholeObject(nullptr);
        }
    }
}