#include "Mover.h"

#include <glm/gtx/transform.hpp>

#include "Object3D.h"
#include "Window.h"

namespace RenderSystem
{
    Mover::Mover(Window* window) :
        Operation(window)
    {}

    void Mover::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        const auto& pickedObject = mWindow->getPickedObject();
        if (mEnabled && pickedObject)
        {
            pickedObject->updateTransform(getTranslationTransform(startCursorPos, endCursorPos));
        }
    }

    void Mover::onKeyPressed(int key)
    {
        if (key == GLFW_KEY_M)
        {
            mEnabled = !mEnabled;
        }
    }

    glm::mat4 Mover::getTranslationTransform(const Point2D& startCursorPos, const Point2D& endCursorPos) const
    {
        const auto& startPosInWorldSpace = mWindow->unProject(startCursorPos);
        const auto& endPosInWorldSpace = mWindow->unProject(endCursorPos);

        if (mWindow->getProjectionType() == PROJECTION_TYPE::PERSPECTIVE)
        {
            return glm::translate(mWindow->projectToCameraTargetPlane(endPosInWorldSpace) - mWindow->projectToCameraTargetPlane(startPosInWorldSpace));
        }
        else
        {
            return glm::translate(endPosInWorldSpace - startPosInWorldSpace);
        }
    }
}