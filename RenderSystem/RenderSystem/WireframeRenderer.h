#pragma once

#include "GeometryCore/Typedefs.h"

#include "Operation.h"

namespace RenderSystem
{
    using namespace GeometryCore;

    class Scene;

    class WireframeRenderer : public Operation
    {
    public:
        WireframeRenderer(Scene* scene);

        void toggle() override;
        void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override {};
        void onMouseClick(const Point2D& cursorPos) override {};
    };
}
