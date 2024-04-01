#pragma once

#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"

namespace RenderSystem
{
    using namespace GeometryCore;

    class Scene;

    class Operation
    {
    public:
        Operation(Scene* scene);
        virtual ~Operation() = default;

        virtual void toggle() = 0;
        virtual void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) = 0;
        virtual void onMouseClick(const Point2D& cursorPos) = 0;

    protected:
        Point3D unProject(const Point2D& cursorPos) const;

    protected:
        bool mEnabled = false;
        Scene* mScene = nullptr;
    };
}
