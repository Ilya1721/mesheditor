#pragma once

#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"

namespace RenderSystem
{
    using namespace GeometryCore;

    class Window;

    class Operation
    {
    public:
        Operation(Window* window);
        virtual ~Operation() = default;

        virtual void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) = 0;
        virtual void onMouseScroll(double offset) = 0;
        virtual void onMouseClick() = 0;
        virtual void onKeyPressed(int key) = 0;

    protected:
        bool mEnabled = false;
        Window* mWindow = nullptr;
    };
}
