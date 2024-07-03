#include "Pan.h"

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    Pan::Pan(Scene* scene)
        : Operation(scene)
    {}

    void Pan::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        const auto& window = mScene->getParentWindow();
        auto middleMouseButtonPressed = window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE);
        auto shiftKeyPressed = window->isKeyPressed(GLFW_KEY_LEFT_SHIFT);

        if (mScene->isCameraMovementEnabled() && middleMouseButtonPressed && shiftKeyPressed)
        {
            const auto& viewport = window->getViewport();
            mScene->getCamera().pan(window->unProject(startCursorPos), window->unProject(endCursorPos), viewport->getProjectionType());
        }
    }
}