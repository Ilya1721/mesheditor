#pragma once

#include <vector>

namespace RenderSystem
{
    class Object3D;

    struct HighlightedFacesData
    {
        std::vector<int> facesIndices;
        const Object3D* parentObject = nullptr;
    };

    class GlobalRenderState
    {
    public:
        ~GlobalRenderState() = delete;

        static bool getRenderWireframe();
        static const Object3D* getHighlightedObject();
        static const HighlightedFacesData& getHighlightedFacesData();

        static void toggleWireframe();
        static void highlightWholeObject(const Object3D* object);
        static void setHighlightedFacesData(const HighlightedFacesData& data);

    private:
        static bool gRenderWireframe;
        static const Object3D* gHighlightedObject;
        static HighlightedFacesData gHighlightedFacesData;
    };
}
