#pragma once

#include <vector>

namespace MeshCore
{
    class Object3D;
}

namespace RenderSystem
{
    class GlobalRenderState
    {
    public:
        ~GlobalRenderState() = delete;

        static bool getRenderWireframe();
        static MeshCore::Object3D* getHighlightedObject();
        static const std::vector<int>& getHighlightedFacesIndices();

        static void toggleWireframe();
        static void highlightWholeObject(MeshCore::Object3D* object);
        static void setHighlightedFaces(const std::vector<int>& facesIndices);

    private:
        static bool gRenderWireframe;
        static MeshCore::Object3D* gHighlightedObject;
        static std::vector<int> gHighlightedFacesIndices;
    };
}
