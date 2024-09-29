#pragma once

#include <functional>

#include "MeshCore/Vertex.h"

#include "RenderData.h"

namespace RenderSystem
{
    class Object3D;

    class RootRenderDataStorage
    {
    public:
        ~RootRenderDataStorage() = delete;

        static const RenderData& getRenderData();
        static const RenderData& getExtraRenderData();

        static void updateRenderData(const std::unordered_set<UniqueVertex*>& vertices, int startOffset);
        static void appendRenderData(const RenderData& renderData);
        static void appendExtraRenderData(const RenderData& renderData);

        static void addOnRenderDataUpdatedCallback(const std::function<void()>& callback);
        static void addOnExtraRenderDataUpdatedCallback(const std::function<void()>& callback);

    private:
        static RenderData gRenderData;
        static RenderData gExtraRenderData;
        static std::vector<std::function<void()>> gOnRenderDataUpdatedCallbacks;
        static std::vector<std::function<void()>> gOnExtraRenderDataUpdatedCallbacks;
    };
}
