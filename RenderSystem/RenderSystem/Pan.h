#pragma once

#include "GeometryCore/Typedefs.h"

#include "Operation.h"

namespace RenderSystem
{
    using namespace GeometryCore;

    class Pan : public Operation
    {
    public:
        Pan(Window* window);

        void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override;
        void onMouseScroll(double offset) override {};
        void onMouseClick() override {};
        void onKeyPressed(int key) override {};
    };
}