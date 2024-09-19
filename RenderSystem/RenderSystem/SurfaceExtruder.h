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
        SurfaceExtruder();

        void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override;
        void onMouseScroll(double offset) override {};
        void onMouseClick() override;
        void onKeyPressed(int key) override;

    private:
        void toggleSurfaceMovement(bool isSurfaceIntersected = false);
        void highlightIntersectedSurface() const;

    private:
        MeshCore::RaySurfaceIntersection mSurfaceIntersection;
        bool mSurfaceMovementEnabled;
    };
}
