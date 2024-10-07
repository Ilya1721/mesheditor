#include "WireframeRenderer.h"

#include "Window.h"

namespace RenderSystem
{
    WireframeRenderer::WireframeRenderer(Window* window) :
        Operation(window)
    {}

    void WireframeRenderer::onKeyPressed(int key)
    {
        if (key == GLFW_KEY_W)
        {
            mWindow->toggleWireframe();
        }
    }
}