#include "OperationsDispatcher.h"

#include <glfw/glfw3.h>

#include "SurfaceHighlighter.h"
#include "SurfaceExtruder.h"
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
            auto& [key, operation] = *operationIt;
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

    void OperationsDispatcher::onMouseClick(const Point2D& cursorPos)
    {
        for (auto& [key, operation] : mKeyOperationMap)
        {
            operation->onMouseClick(cursorPos);
        }
    }

    void OperationsDispatcher::initSurfaceOperations()
    {
        addOperation<SurfaceHighlighter>(GLFW_KEY_H);
        addOperation<SurfaceExtruder>(GLFW_KEY_E);
    }
}