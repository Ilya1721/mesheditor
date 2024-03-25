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

    OperationsDispatcher::~OperationsDispatcher() = default;

    void OperationsDispatcher::toggle(int key)
    {
        auto operationIt = mKeyOperationMap.find(key);
        if (operationIt != mKeyOperationMap.end())
        {
            auto& [key, operation] = *operationIt;
            operation->toggle();
        }
    }

    void OperationsDispatcher::onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos)
    {
        for (auto& [key, operation] : mKeyOperationMap)
        {
            operation->onMouseMove(startCursorPos, endCursorPos);
        }
    }

    void OperationsDispatcher::onMouseClick(const glm::vec2& cursorPos)
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