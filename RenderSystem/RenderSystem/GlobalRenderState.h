#pragma once

#include <vector>

namespace MeshCore
{
    class Object3D;
}

namespace RenderSystem
{
    struct HighlightedFacesData
    {
        std::vector<int> facesIndices;
        MeshCore::Object3D* parentObject;
    };

    class GlobalRenderState
    {
    public:
        ~GlobalRenderState() = delete;

        static bool getRenderWireframe();
        static MeshCore::Object3D* getHighlightedObject();
        static const HighlightedFacesData& getHighlightedFacesData();

        static void toggleWireframe();
        static void highlightWholeObject(MeshCore::Object3D* object);
        static void setHighlightedFacesData(const HighlightedFacesData& data);

    private:
        static bool gRenderWireframe;
        static MeshCore::Object3D* gHighlightedObject;
        static HighlightedFacesData gHighlightedFacesData;
    };
}
