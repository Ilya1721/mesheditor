#include "WireframeRenderer.h"

#include "Scene.h"

namespace RenderSystem
{
    WireframeRenderer::WireframeRenderer(Scene* scene) :
        Operation(scene)
    {}

    void WireframeRenderer::toggle()
    {
        mScene->getRenderer().toggleWireframe();
    }
}