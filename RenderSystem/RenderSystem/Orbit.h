#pragma once

#include "GeometryCore/Typedefs.h"

#include "Operation.h"

namespace RenderSystem
{
    using namespace GeometryCore;

    class Orbit : public Operation
    {
    public:
        Orbit(Scene* scene);

        void toggle() override {};
        void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override;
        void onMouseScroll(double offset) override {};
        void onMouseClick() override {};
    };
}