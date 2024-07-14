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
        if (!mScene->getParentWindow()->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            return;
        }

        auto intersection = mScene->getClosestIntersection();
        if (!intersection.surfaceIndices.empty())
        {
            mScene->setPickedObject(intersection.intersectedSurface.getParentObject());
            mScene->getRenderer().highlightWholeObject(true);
        }
        else
        {
            mScene->getRenderer().highlightWholeObject(false);
        }
    }
}