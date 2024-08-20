#include "RootRenderDataStorage.h"

#include "Object3D.h"

namespace MeshCore
{
    RenderData RootRenderDataStorage::gRenderData;
    RenderData RootRenderDataStorage::gExtraRenderData;
    std::vector<std::function<void()>> RootRenderDataStorage::gOnRenderDataUpdatedCallbacks;
    std::vector<std::function<void()>> RootRenderDataStorage::gOnExtraRenderDataUpdatedCallbacks;

    void fireCallbacks(const std::vector<std::function<void()>>& callbacks)
    {
        for (const auto& callback : callbacks)
        {
            callback();
        }
    }
}

namespace MeshCore
{
    const RenderData& RootRenderDataStorage::getRenderData()
    {
        return gRenderData;
    }

    const RenderData& RootRenderDataStorage::getExtraRenderData()
    {
        return gExtraRenderData;
    }

    void RootRenderDataStorage::updateRenderData(const std::unordered_set<UniqueVertex*>& vertices, int startVertexOffset)
    {
        for (auto& vertex : vertices)
        {
            for (auto& originalVertexData : vertex->originalVertices)
            {
                gRenderData.updateVertex(originalVertexData, startVertexOffset);
            }
        }
        fireCallbacks(gOnRenderDataUpdatedCallbacks);
    }

    void RootRenderDataStorage::addOnRenderDataUpdatedCallback(const std::function<void()>& callback)
    {
        gOnRenderDataUpdatedCallbacks.push_back(callback);
    }

    void RootRenderDataStorage::addOnExtraRenderDataUpdatedCallback(const std::function<void()>& callback)
    {
        gOnExtraRenderDataUpdatedCallbacks.push_back(callback);
    }

    void RootRenderDataStorage::appendRenderData(const RenderData& renderData)
    {
        gRenderData.append(renderData);
        fireCallbacks(gOnRenderDataUpdatedCallbacks);
    }

    void RootRenderDataStorage::appendExtraRenderData(const RenderData& renderData)
    {
        gExtraRenderData.append(renderData);
        fireCallbacks(gOnExtraRenderDataUpdatedCallbacks);
    }
}