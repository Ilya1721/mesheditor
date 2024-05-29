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
        void onMouseClick() override;

    private:
        MeshCore::RaySurfaceIntersection mSurfaceIntersection;
    };
}
