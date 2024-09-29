#pragma once

#include "Intersection.h"

namespace RenderSystem
{
    class Object3D;

    struct Object3DIntersectionData
    {
        Object3D* intersectedObject;
        RaySurfaceIntersection intersection;
    };
}
