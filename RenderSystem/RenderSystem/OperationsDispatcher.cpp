#include "OperationsDispatcher.h"

#include <glfw/glfw3.h>

#include "SurfaceHighlighter.h"
#include "SurfaceExtruder.h"
#include "WireframeRenderer.h"
#include "Operation.h"

namespace RenderSystem
{
    OperationsDispatcher::OperationsDispatcher(Scene* scene) :
        mScene(scene)
    {
        initSurfaceOperations();
    }

    void OperationsDispatcher::toggle(int key)
    {
        auto operationIt = mKeyOperationMap.find(key);
        if (operationIt != mKeyOperationMap.end())
        {
            auto& [_, operation] = *operationIt;
            operation->toggle();
        }
    }

    void OperationsDispatcher::onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos)
    {
        for (auto& [key, operation] : mKeyOperationMap)
        {
            operation->onMouseMove(startCursorPos, endCursorPos);
        }
    }

    void OperationsDispatcher::onMouseClick()
    {
        for (auto& [key, operation] : mKeyOperationMap)
        {
            operation->onMouseClick();
        }
    }

    void OperationsDispatcher::initSurfaceOperations()
    {
        addOperation<SurfaceHighlighter>(GLFW_KEY_H);
        addOperation<SurfaceExtruder>(GLFW_KEY_E);
        addOperation<WireframeRenderer>(GLFW_KEY_W);
    }
}