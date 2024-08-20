#include "GlobalRenderState.h"

#include "MeshCore/Object3D.h"

namespace RenderSystem
{
    bool GlobalRenderState::gRenderWireframe = false;
    MeshCore::Object3D* GlobalRenderState::gHighlightedObject = nullptr;
    HighlightedFacesData GlobalRenderState::gHighlightedFacesData;
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

    const HighlightedFacesData& GlobalRenderState::getHighlightedFacesData()
    {
        return gHighlightedFacesData;
    }

    void GlobalRenderState::toggleWireframe()
    {
        gRenderWireframe = !gRenderWireframe;
    }

    void GlobalRenderState::highlightWholeObject(MeshCore::Object3D* object)
    {
        gHighlightedObject = object;
    }

    void GlobalRenderState::setHighlightedFacesData(const HighlightedFacesData& data)
    {
        gHighlightedFacesData = data;
    }
}