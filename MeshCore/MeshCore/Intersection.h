#pragma once

#include <optional>
#include <vector>

#include "Surface.h"

namespace MeshCore
{
  struct MeshIntersectionTempData
  {
    std::optional<Point3D> intersectionPoint;
    int intersectedFaceIdx = -1;
  };

  struct MeshIntersection
  {
    std::vector<int> intersectedFacesIndices;
    Point3D intersectionPoint {};
    Surface intersectedSurface {};
  };

  enum class IntersectionMode
  {
    SURFACE,
    FACE
  };
}  // namespace MeshCore
