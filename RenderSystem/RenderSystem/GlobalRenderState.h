#pragma once

#include <vector>
#include <memory>
#include <string>
#include <functional>

#include "Utility/CallbackMechanism.h"

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
        static GlobalRenderState& getInstance();

        bool getRenderWireframe() const;
        MeshCore::Object3D* getHighlightedObject();
        MeshCore::Object3D* getPickedObject();
        const HighlightedFacesData& getHighlightedFacesData();
        const std::unique_ptr<MeshCore::Object3D>& getRootObject();

        void toggleWireframe();
        void highlightWholeObject(MeshCore::Object3D* object);
        void setPickedObject(MeshCore::Object3D* object);
        void setHighlightedFacesData(const HighlightedFacesData& data);
        void addRootObjectInitializedCallback(const std::function<void()>& callback);

    private:
        GlobalRenderState();
        void initRootObject(const std::string& meshFilePath);

    private:
        bool mRenderWireframe;
        MeshCore::Object3D* mHighlightedObject;
        MeshCore::Object3D* mPickedObject;
        HighlightedFacesData mHighlightedFacesData;
        std::unique_ptr<MeshCore::Object3D> mRootObject;
        Utility::CallbackMechanism mRootObjectInitCM;
    };
}
