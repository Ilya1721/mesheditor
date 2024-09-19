#include "Pan.h"

#include "Camera.h"
#include "Window.h"
#include "Viewport.h"

namespace
{
    using namespace RenderSystem;

    Window* gWindow = &Window::getInstance();
    Camera* gCamera = &Camera::getInstance();
    Viewport* gViewport = &Viewport::getInstance();
}

namespace RenderSystem
{
    void Pan::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        auto middleMouseButtonPressed = gWindow->isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE);
        auto shiftKeyPressed = gWindow->isKeyPressed(GLFW_KEY_LEFT_SHIFT);

        if (gCamera->isMovementEnabled() && middleMouseButtonPressed && shiftKeyPressed)
        {
            gCamera->pan(gWindow->unProject(startCursorPos), gWindow->unProject(endCursorPos), gViewport->getProjectionType());
        }
    }
}