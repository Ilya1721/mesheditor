#include "Operation.h"

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    Operation::Operation(Scene* scene) :
        mScene(scene)
    {}

    glm::vec3 Operation::unProject(const glm::vec2& cursorPos) const
    {
        auto parentWindow = mScene->getParentWindow();
        return parentWindow->unProject(parentWindow->getCursorPos());
    }
}