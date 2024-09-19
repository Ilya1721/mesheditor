#include "WireframeRenderer.h"

#include "Window.h"
#include "GlobalRenderState.h"

namespace
{
    using namespace RenderSystem;

    GlobalRenderState* gGlobalRenderState = &GlobalRenderState::getInstance();
}

namespace RenderSystem
{
    void WireframeRenderer::onKeyPressed(int key)
    {
        if (key == GLFW_KEY_W)
        {
            gGlobalRenderState->toggleWireframe();
        }
    }
}