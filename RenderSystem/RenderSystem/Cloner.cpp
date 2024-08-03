#include "Cloner.h"

#include <glm/gtx/transform.hpp>

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    Cloner::Cloner(Scene* scene) :
        Operation(scene)
    {}

    void Cloner::onKeyPressed(int key)
    {
        auto pickedObject = mScene->getPickedObject();
        if (key == GLFW_KEY_C && pickedObject)
        {
            pickedObject->getParent()->addChild(pickedObject->clone(getInitialTransform(pickedObject->getBBox())));
            mScene->setPickedObject(nullptr);
            mScene->getRenderer().highlightWholeObject(nullptr);
            mScene->updateRenderBuffer();
        }
    }

    glm::mat4 Cloner::getInitialTransform(const MeshCore::AABBox& bbox) const
    {
        auto distance = 2.0f * (bbox.getMax().x - bbox.getMin().x);
        return glm::translate(Vector3D(distance, 0.0f, 0.0f));
    }
}