#pragma once

#include <unordered_map>
#include <memory>
#include <concepts>

#include "MeshCore/Intersection.h"

#include "Operation.h"

namespace RenderSystem
{
    class OperationsDispatcher
    {
    public:
        OperationsDispatcher(Scene* scene);

        void toggle(int key);
        void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos);
        void onMouseClick(const glm::vec2& cursorPos);

    private:
        void initSurfaceOperations();
        template<typename T>
        void addOperation(int key) requires std::derived_from<T, Operation>
        {
            mKeyOperationMap.insert({ key, std::make_unique<T>(mScene) });
        }

    private:
        std::unordered_map<int, std::unique_ptr<Operation>> mKeyOperationMap;
        Scene* mScene;
    };
}
