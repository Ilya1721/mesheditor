#pragma once

#include <glm/glm.hpp>

#include "Typedefs.h"

namespace GeometryCore
{
  struct Line
  {
    Point3D start;
    Point3D end;

    glm::mat4 getTransformToSelf(const Line& input) const;
  };
}  // namespace GeometryCore
