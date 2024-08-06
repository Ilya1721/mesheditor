#include "WireframeRenderer.h"

#include "Scene.h"
#include "Window.h"
#include "GlobalRenderState.h"

namespace RenderSystem
{
    WireframeRenderer::WireframeRenderer(Scene* scene) :
        Operation(scene)
    {}

    void WireframeRenderer::onKeyPressed(int key)
    {
        if (key == GLFW_KEY_W)
        {
            GlobalRenderState::toggleWireframe();
        }
    }
}