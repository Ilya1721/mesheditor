#pragma once

#include <functional>

#include "Utility/CallbackMechanism.h"

#include "RenderData.h"
#include "Vertex.h"

namespace MeshCore
{
    class Object3D;

    class RootRenderDataStorage
    {
    public:
        static RootRenderDataStorage& getInstance();

        const RenderData& getRenderData();
        const RenderData& getExtraRenderData();

        void updateRenderData(const std::unordered_set<UniqueVertex*>& vertices, int startOffset);
        void appendRenderData(const RenderData& renderData);
        void appendExtraRenderData(const RenderData& renderData);

        void addOnRenderDataUpdatedCallback(const std::function<void()>& callback);
        void addOnExtraRenderDataUpdatedCallback(const std::function<void()>& callback);

    private:
        ~RootRenderDataStorage() = default;

    private:
        RenderData mRenderData;
        RenderData mExtraRenderData;
        Utility::CallbackMechanism mRenderDataUpdatedCM;
        Utility::CallbackMechanism mExtraRenderDataUpdatedCM;
    };
}
