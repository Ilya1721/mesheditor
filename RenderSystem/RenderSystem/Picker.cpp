#include "Picker.h"

#include "MeshCore/Intersection.h"
#include "MeshCore/Object3D.h"

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    Picker::Picker(Scene* scene) :
        Operation(scene)
    {}

    void Picker::onMouseClick()
    {
        if (!mScene->getParentWindow()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) ||
            !mScene->isCameraMovementEnabled())
        {
            return;
        }

        auto intersection = mScene->getClosestIntersection();
        if (!intersection.surfaceIndices.empty())
        {
            auto pickedObject = intersection.intersectedSurface.getParentObject();
            mScene->setPickedObject(pickedObject);
            mScene->getRenderer().highlightWholeObject(pickedObject);
        }
        else
        {
            mScene->setPickedObject(nullptr);
            mScene->getRenderer().highlightWholeObject(nullptr);
        }
    }
}