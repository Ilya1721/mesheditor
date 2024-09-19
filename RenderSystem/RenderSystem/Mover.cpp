#include "Mover.h"

#include <glm/gtx/transform.hpp>

#include "MeshCore/Object3D.h"

#include "Window.h"
#include "GlobalRenderState.h"
#include "Viewport.h"
#include "Camera.h"

namespace
{
    using namespace RenderSystem;

    Window* gWindow = &Window::getInstance();
    GlobalRenderState* gGlobalRenderState = &GlobalRenderState::getInstance();
    Viewport* gViewport = &Viewport::getInstance();
    Camera* gCamera = &Camera::getInstance();
}

namespace RenderSystem
{
    void Mover::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        const auto& pickedObject = gGlobalRenderState->getPickedObject();
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
        const auto& startPosInWorldSpace = gWindow->unProject(startCursorPos);
        const auto& endPosInWorldSpace = gWindow->unProject(endCursorPos);

        if (gViewport->getProjectionType() == PROJECTION_TYPE::PERSPECTIVE)
        {
            return glm::translate(gCamera->projectToTargetPlane(endPosInWorldSpace) - gCamera->projectToTargetPlane(startPosInWorldSpace));
        }
        else
        {
            return glm::translate(endPosInWorldSpace - startPosInWorldSpace);
        }
    }
}