#pragma once

#include "GeometryCore/Typedefs.h"

#include "Operation.h"

namespace RenderSystem
{
    class Object3D;

    class Picker : public Operation
    {
    public:
        Picker(Window* window);

        void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override {};
        void onMouseScroll(double offset) override {};
        void onMouseClick() override;
        void onKeyPressed(int key) override {};
    };
}
