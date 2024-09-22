#include "Orbit.h"

#include "Window.h"

namespace RenderSystem
{
    Orbit::Orbit(Window* window) :
        Operation(window)
    {}

    void Orbit::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        auto middleMouseButtonPressed = mWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE);
        auto shiftKeyPressed = mWindow->isKeyPressed(GLFW_KEY_LEFT_SHIFT);

        if (mWindow->isCameraMovementEnabled() && middleMouseButtonPressed && !shiftKeyPressed)
        {
            const auto offsetX = endCursorPos.x - startCursorPos.x;
            const auto offsetY = startCursorPos.y - endCursorPos.y;
            mWindow->smoothOrbit(offsetX, offsetY);
        }
    }
}