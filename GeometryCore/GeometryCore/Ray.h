#pragma once

#include "Typedefs.h"

namespace GeometryCore
{
  struct Ray
  {
    Point3D origin;
    Vector3D direction;

    friend Ray operator*(const glm::mat4& transform, const Ray& ray);
  };
}  // namespace GeometryCore
