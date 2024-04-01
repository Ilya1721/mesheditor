#include "Operation.h"

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    Operation::Operation(Scene* scene) :
        mScene(scene)
    {}

    Point3D Operation::unProject(const Point2D& cursorPos) const
    {
        auto parentWindow = mScene->getParentWindow();
        return parentWindow->unProject(parentWindow->getCursorPos());
    }
}