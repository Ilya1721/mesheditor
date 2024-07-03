#include "OperationsDispatcher.h"

#include <glfw/glfw3.h>

#include "SurfaceHighlighter.h"
#include "SurfaceExtruder.h"
#include "WireframeRenderer.h"
#include "Orbit.h"
#include "Pan.h"
#include "Zoom.h"
#include "Operation.h"

namespace RenderSystem
{
    OperationsDispatcher::OperationsDispatcher(Scene* scene) :
        mScene(scene)
    {
        initSurfaceOperations();
        initCameraMovementOperations();
    }

    void OperationsDispatcher::toggle(int key)
    {
        auto operationIt = mToggleableOperations.find(key);
        if (operationIt != mToggleableOperations.end())
        {
            auto& [_, operation] = *operationIt;
            operation->toggle();
        }
    }

    void OperationsDispatcher::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        for (auto& operation : mOperations)
        {
            operation->onMouseMove(startCursorPos, endCursorPos);
        }
    }

    void OperationsDispatcher::onMouseScroll(double offset)
    {
        for (auto& operation : mOperations)
        {
            operation->onMouseScroll(offset);
        }
    }

    void OperationsDispatcher::onMouseClick()
    {
        for (auto& operation : mOperations)
        {
            operation->onMouseClick();
        }
    }

    void OperationsDispatcher::initSurfaceOperations()
    {
        addToggleableOperation<SurfaceHighlighter>(GLFW_KEY_H);
        addToggleableOperation<SurfaceExtruder>(GLFW_KEY_E);
        addToggleableOperation<WireframeRenderer>(GLFW_KEY_W);
    }

    void OperationsDispatcher::initCameraMovementOperations()
    {
        addBasicOperation<Orbit>();
        addBasicOperation<Pan>();
        addBasicOperation<Zoom>();
    }
}