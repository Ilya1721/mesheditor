#pragma once

#include "GeometryCore/Typedefs.h"

#include "Operation.h"

namespace RenderSystem
{
    class Mover : public Operation
    {
    public:
        Mover(Scene* scene);

        void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override;
        void onMouseScroll(double offset) override {};
        void onMouseClick() override {};
        void onKeyPressed(int key) override;

    private:
        glm::mat4 getTranslationTransform(const Point2D& startCursorPos, const Point2D& endCursorPos) const;
    };
}
