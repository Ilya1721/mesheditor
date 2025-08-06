#pragma once

#include "Intersection.h"

namespace RenderSystem
{
  class Object3D;

  struct Object3DIntersection
  {
    Object3D* intersectedObject = nullptr;
    RaySurfaceIntersection raySurfaceIntersection;
  };
}  // namespace RenderSystem
