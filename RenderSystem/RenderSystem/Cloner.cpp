#include "Cloner.h"

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
            pickedObject->clone();
        }
    }
}