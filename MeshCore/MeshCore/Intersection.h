#pragma once

#include <optional>
#include <vector>

#include "Surface.h"

namespace MeshCore
{
  struct MeshIntersectionTempData
  {
    std::optional<glm::vec3> intersectionPoint;
    int intersectedFaceIdx = -1;
  };

  struct MeshIntersection
  {
    std::vector<int> intersectedFacesIndices;
    glm::vec3 intersectionPoint {};
    Surface intersectedSurface {};
  };

  enum class IntersectionMode
  {
    SURFACE,
    FACE
  };
}  // namespace MeshCore
