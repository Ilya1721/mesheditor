#include "Orbit.h"

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    Orbit::Orbit(Scene* scene) 
        : Operation(scene)
    {}

    void Orbit::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        const auto& window = mScene->getParentWindow();
        auto middleMouseButtonPressed = window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE);
        auto shiftKeyPressed = window->isKeyPressed(GLFW_KEY_LEFT_SHIFT);

        if (mScene->isCameraMovementEnabled() && middleMouseButtonPressed && !shiftKeyPressed)
        {
            mScene->getCamera().orbit(window->screenCoordinatesToNDC(startCursorPos), window->screenCoordinatesToNDC(endCursorPos));
        }
    }
}