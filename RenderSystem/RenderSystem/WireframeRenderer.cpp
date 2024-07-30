#include "WireframeRenderer.h"

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    WireframeRenderer::WireframeRenderer(Scene* scene) :
        Operation(scene)
    {}

    void WireframeRenderer::onKeyPressed(int key)
    {
        if (key == GLFW_KEY_W)
        {
            mScene->getRenderer().toggleWireframe();
        }
    }
}