#include "GlobalRenderState.h"

#include "MeshCore/Object3D.h"
#include "MeshCore/Mesh.h"
#include "MeshFilesLoader/MeshFilesLoader.h"

#include "GlobalExtraPrimitives.h"
#include "Window.h"

namespace
{
    using namespace RenderSystem;

    Window* gWindow = nullptr;
}

namespace RenderSystem
{
    GlobalRenderState::GlobalRenderState()
    {
        gWindow = &Window::getInstance();
        gWindow->addInitializedCallback([this]() {
            initRootObject(gWindow->getMeshFilePath());
        });
    }

    GlobalRenderState& GlobalRenderState::getInstance()
    {
        static GlobalRenderState sInstance;
        return sInstance;
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

    void GlobalRenderState::initRootObject(const std::string& meshFilePath)
    {
        auto firstSceneObject = std::make_unique<MeshCore::Object3D>(MeshFilesLoader::loadSTL(meshFilePath));
        firstSceneObject->moveToOrigin();
        mRootObject->addChild(std::move(firstSceneObject));
        mRootObjectInitCM.invokeCallbacks();
    }

    void GlobalRenderState::addRootObjectInitializedCallback(const std::function<void()>& callback)
    {
        mRootObjectInitCM.addCallback(callback);
    }
}