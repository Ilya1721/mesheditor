#pragma once

#include "MeshCore/Intersection.h"
#include "GeometryCore/Typedefs.h"

#include "Operation.h"

namespace RenderSystem
{
    using namespace GeometryCore;

    class Scene;

    class SurfaceExtruder : public Operation
    {
    public:
        SurfaceExtruder(Scene* scene);

        void toggle() override;
        void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override;
        void onMouseScroll(double offset) override {};
        void onMouseClick() override;

    private:
        void toggleSurfaceMovement(bool isSurfaceIntersected = false);
        void highlightIntersectedSurface();

    private:
        MeshCore::RaySurfaceIntersection mSurfaceIntersection;
        bool mSurfaceMovementEnabled;
    };
}
