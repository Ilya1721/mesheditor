#include "Operation.h"

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    Operation::Operation(Scene* scene) :
        mScene(scene)
    {}

    Point3D Operation::unProjectCursorPos() const
    {
        auto parentWindow = mScene->getParentWindow();
        return parentWindow->unProject(parentWindow->getCursorPos());
    }
}