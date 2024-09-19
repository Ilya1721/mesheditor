#include "Orbit.h"

#include "Window.h"
#include "Camera.h"

namespace 
{
    using namespace RenderSystem;

    Window* gWindow = &Window::getInstance();
    Camera* gCamera = &Camera::getInstance();
}

namespace RenderSystem
{
    void Orbit::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        auto middleMouseButtonPressed = gWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE);
        auto shiftKeyPressed = gWindow->isKeyPressed(GLFW_KEY_LEFT_SHIFT);

        if (gCamera->isMovementEnabled() && middleMouseButtonPressed && !shiftKeyPressed)
        {
            const auto offsetX = endCursorPos.x - startCursorPos.x;
            const auto offsetY = startCursorPos.y - endCursorPos.y;
            gCamera->smoothOrbit(offsetX, offsetY);
        }
    }
}