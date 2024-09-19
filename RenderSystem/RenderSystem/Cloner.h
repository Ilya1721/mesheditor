#pragma once

#include "GeometryCore/Typedefs.h"
#include "MeshCore/AABBox.h"

#include "Operation.h"

namespace RenderSystem
{
    class Cloner : public Operation
    {
    public:
        void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override {};
        void onMouseScroll(double offset) override {};
        void onMouseClick() override {};
        void onKeyPressed(int key) override;

    private:
        glm::mat4 getInitialTransform(const MeshCore::AABBox& bbox) const;
    };
}
