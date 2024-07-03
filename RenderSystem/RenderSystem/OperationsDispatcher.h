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
        void onMouseScroll(double offset);
        void onMouseClick();

    private:
        void initSurfaceOperations();
        void initCameraMovementOperations();

        template<typename T>
        void addToggleableOperation(int key) requires std::derived_from<T, Operation>
        {
            addBasicOperation<T>();
            mToggleableOperations.insert({ key, mOperations.back().get() });
        }

        template<typename T>
        void addBasicOperation() requires std::derived_from<T, Operation>
        {
            mOperations.emplace_back(std::make_unique<T>(mScene));
        }

    private:
        std::unordered_map<int, Operation*> mToggleableOperations;
        std::vector<std::unique_ptr<Operation>> mOperations;
        Scene* mScene;
    };
}
