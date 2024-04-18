#pragma once

#include <unordered_map>
#include <memory>
#include <concepts>

#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"

#include "Operation.h"

namespace RenderSystem
{
    using namespace GeometryCore;

    class Scene;

    class OperationsDispatcher
    {
    public:
        OperationsDispatcher(Scene* scene);

        void toggle(int key);
        void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos);
        void onMouseClick(const Point2D& cursorPos);

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
