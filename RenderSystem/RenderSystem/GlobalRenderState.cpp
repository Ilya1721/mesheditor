#include "GlobalRenderState.h"

#include "MeshCore/Object3D.h"
#include "MeshFilesLoader/MeshFilesLoader.h"

#include "GlobalExtraPrimitives.h"
#include "Window.h"

namespace RenderSystem
{
    GlobalRenderState GlobalRenderState::gInstance;
}

namespace RenderSystem
{
    GlobalRenderState::GlobalRenderState()
    {
        addWindowInitializedCallbacks();
    }

    GlobalRenderState& GlobalRenderState::getInstance()
    {
        return gInstance;
    }

    void GlobalRenderState::addWindowInitializedCallbacks()
    {

    }

    bool GlobalRenderState::getRenderWireframe() const
    {
        return mRenderWireframe;
    }

    MeshCore::Object3D* GlobalRenderState::getHighlightedObject()
    {
        return mHighlightedObject;
    }

    MeshCore::Object3D* GlobalRenderState::getPickedObject()
    {
        return mPickedObject;
    }

    const HighlightedFacesData& GlobalRenderState::getHighlightedFacesData()
    {
        return mHighlightedFacesData;
    }

    const std::unique_ptr<MeshCore::Object3D>& GlobalRenderState::getRootObject()
    {
        if (!mRootObject)
        {
            throw std::exception("The root object is not initialized yet");
        }

        return mRootObject;
    }

    void GlobalRenderState::toggleWireframe()
    {
        mRenderWireframe = !mRenderWireframe;
    }

    void GlobalRenderState::highlightWholeObject(MeshCore::Object3D* object)
    {
        mHighlightedObject = object;
    }

    void GlobalRenderState::setPickedObject(MeshCore::Object3D* object)
    {
        mPickedObject = object;
    }

    void GlobalRenderState::setHighlightedFacesData(const HighlightedFacesData& data)
    {
        mHighlightedFacesData = data;
    }

    void GlobalRenderState::initializeRootObject(const std::string& meshFilePath)
    {
        auto firstSceneObject = std::make_unique<MeshCore::Object3D>(MeshFilesLoader::loadSTL(meshFilePath));
        firstSceneObject->moveToOrigin();
        mRootObject->addChild(std::move(firstSceneObject));
        //GlobalExtraPrimitives::addSceneFloor();
        for (const auto& callback : mRootObjectInitializedCallbacks)
        {
            callback();
        }
    }

    void GlobalRenderState::addRootObjectInitializedCallback(const std::function<void()>& callback)
    {
        mRootObjectInitializedCallbacks.push_back(callback);
    }
}