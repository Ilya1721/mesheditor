#pragma once

#include "GeometryCore/Typedefs.h"

#include "Operation.h"

namespace RenderSystem
{
    class Cloner : public Operation
    {
    public:
        Cloner(Scene* scene);

        void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override {};
        void onMouseScroll(double offset) override {};
        void onMouseClick() override {};
        void onKeyPressed(int key) override;
    };
}
