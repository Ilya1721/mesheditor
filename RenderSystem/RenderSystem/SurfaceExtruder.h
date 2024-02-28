#pragma once

#include "MeshCore/Intersection.h"

#include "Operation.h"

namespace RenderSystem
{
    class Scene;

    class SurfaceExtruder : public Operation
    {
    public:
        SurfaceExtruder(Scene* scene);

        void toggle() override;
        void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos) override;
        void onMouseClick(const glm::vec2& cursorPos) override;

    private:
        MeshCore::RaySurfaceIntersection mSurfaceIntersection;
    };
}
