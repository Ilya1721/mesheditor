#include "GlobalRenderState.h"

#include "MeshCore/Object3D.h"

namespace RenderSystem
{
    bool GlobalRenderState::gRenderWireframe = false;
    MeshCore::Object3D* GlobalRenderState::gHighlightedObject = nullptr;
    std::vector<int> GlobalRenderState::gHighlightedFacesIndices;
}

namespace RenderSystem
{
    bool GlobalRenderState::getRenderWireframe()
    {
        return gRenderWireframe;
    }

    MeshCore::Object3D* GlobalRenderState::getHighlightedObject()
    {
        return gHighlightedObject;
    }

    const std::vector<int>& GlobalRenderState::getHighlightedFacesIndices()
    {
        return gHighlightedFacesIndices;
    }

    void GlobalRenderState::toggleWireframe()
    {
        gRenderWireframe = !gRenderWireframe;
    }

    void GlobalRenderState::highlightWholeObject(MeshCore::Object3D* object)
    {
        gHighlightedObject = object;
    }

    void GlobalRenderState::setHighlightedFaces(const std::vector<int>& facesIndices)
    {
        gHighlightedFacesIndices = facesIndices;
    }
}