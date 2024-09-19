#include "RootRenderDataStorage.h"

#include "Object3D.h"

namespace MeshCore
{
    RootRenderDataStorage& RootRenderDataStorage::getInstance()
    {
        static RootRenderDataStorage sInstance;
        return sInstance;
    }

    const RenderData& RootRenderDataStorage::getRenderData()
    {
        return mRenderData;
    }

    const RenderData& RootRenderDataStorage::getExtraRenderData()
    {
        return mExtraRenderData;
    }

    void RootRenderDataStorage::updateRenderData(const std::unordered_set<UniqueVertex*>& vertices, int startVertexOffset)
    {
        for (auto& vertex : vertices)
        {
            for (auto& originalVertexData : vertex->originalVertices)
            {
                mRenderData.updateVertex(originalVertexData, startVertexOffset);
            }
        }
        mRenderDataUpdatedCM.invokeCallbacks();
    }

    void RootRenderDataStorage::addOnRenderDataUpdatedCallback(const std::function<void()>& callback)
    {
        mRenderDataUpdatedCM.addCallback(callback);
    }

    void RootRenderDataStorage::addOnExtraRenderDataUpdatedCallback(const std::function<void()>& callback)
    {
        mExtraRenderDataUpdatedCM.addCallback(callback);
    }

    void RootRenderDataStorage::appendRenderData(const RenderData& renderData)
    {
        mRenderData.append(renderData);
        mRenderDataUpdatedCM.invokeCallbacks();
    }

    void RootRenderDataStorage::appendExtraRenderData(const RenderData& renderData)
    {
        mExtraRenderData.append(renderData);
        mExtraRenderDataUpdatedCM.invokeCallbacks();
    }
}