#pragma once

#include "GeometryCore/Typedefs.h"

#include "Operation.h"

namespace RenderSystem
{
    using namespace GeometryCore;

    class WireframeRenderer : public Operation
    {
    public:
        WireframeRenderer(Window* window);

        void onMouseMove(const Point2D&, const Point2D&) override {};
        void onMouseScroll(double offset) override {};
        void onMouseClick() override {};
        void onKeyPressed(int key) override;
    };
}
