#include "Cloner.h"

#include <glm/gtx/transform.hpp>

#include "Object3D.h"
#include "Window.h"

namespace RenderSystem
{
    Cloner::Cloner(Window* window) :
        Operation(window)
    {}

    void Cloner::onKeyPressed(int key)
    {
        auto pickedObject = mWindow->getPickedObject();
        if (key == GLFW_KEY_C && pickedObject)
        {
            auto clonedObject = pickedObject->clone();
            clonedObject->updateTransform(getInitialTransform(pickedObject->getBBox()));
            pickedObject->getParent()->addChild(std::move(clonedObject));
        }
    }

    glm::mat4 Cloner::getInitialTransform(const MeshCore::AABBox& bbox) const
    {
        auto distance = 2.0f * (bbox.getMax().x - bbox.getMin().x);
        return glm::translate(Vector3D(distance, 0.0f, 0.0f));
    }
}