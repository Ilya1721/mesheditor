#include "Mover.h"

#include <glm/gtx/transform.hpp>

#include "Scene.h"
#include "Window.h"

namespace RenderSystem
{
    Mover::Mover(Scene* scene) :
        Operation(scene)
    {}

    void Mover::toggle()
    {
        mEnabled = !mEnabled;
    }

    void Mover::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        const auto& pickedObject = mScene->getPickedObject();
        if (mEnabled && pickedObject)
        {
            pickedObject->updateTransform(getTranslationTransform(startCursorPos, endCursorPos));
        }
    }

    glm::mat4 Mover::getTranslationTransform(const Point2D& startCursorPos, const Point2D& endCursorPos) const
    {
        const auto& window = mScene->getParentWindow();
        const auto& startPosInWorldSpace = window->unProject(startCursorPos);
        const auto& endPosInWorldSpace = window->unProject(endCursorPos);

        if (window->getViewport()->getProjectionType() == PROJECTION_TYPE::PERSPECTIVE)
        {
            const auto& camera = mScene->getCamera();
            return glm::translate(camera.projectToTargetPlane(endPosInWorldSpace) - camera.projectToTargetPlane(startPosInWorldSpace));
        }
        else
        {
            return glm::translate(endPosInWorldSpace - startPosInWorldSpace);
        }
    }
}