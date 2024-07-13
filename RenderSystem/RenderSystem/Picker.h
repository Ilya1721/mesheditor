#pragma once

#include "GeometryCore/Typedefs.h"

#include "Operation.h"

namespace MeshCore
{
    class Object3D;
}

namespace RenderSystem
{
    class Picker : public Operation
    {
    public:
        Picker(Scene* scene);

        void toggle() override {};
        void onMouseMove(const Point2D& startCursorPos, const Point2D& endCursorPos) override {};
        void onMouseScroll(double offset) override {};
        void onMouseClick() override;

    private:
        MeshCore::Object3D* mPickedObject;
    };
}
