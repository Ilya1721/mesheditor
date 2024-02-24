#pragma once

#include "MeshCore/Intersection.h"

#include "Operation.h"

namespace RenderSystem
{
    class Scene;

    class SurfaceHighlighter : public Operation
    {
    public:
        SurfaceHighlighter(Scene* scene);

        void onMouseMove(const glm::vec2& startCursorPos, const glm::vec2& endCursorPos) override;
        void onMouseClick(const glm::vec2& cursorPos) override {};
    };
}
