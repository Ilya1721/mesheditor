#include "Picker.h"

#include "MeshCore/Intersection.h"

#include "Object3D.h"
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

        auto intersectionData = mWindow->getClosestIntersection();
        if (!intersectionData.intersection.surfaceIndices.empty())
        {
            auto pickedObject = intersectionData.intersectedObject;
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