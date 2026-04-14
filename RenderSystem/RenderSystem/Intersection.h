#pragma once

#include <vector>

#include "MeshCore/Surface.h"

namespace MeshCore
{
  struct MeshIntersection;
}

namespace RenderSystem
{
  using namespace GeometryCore;

  struct RaySurfaceIntersection
  {
    MeshCore::Surface surface {};
    std::vector<int> surfaceIndices;
    glm::vec3 point {};

    void assign(const MeshCore::MeshIntersection& meshIntersection);
  };
}  // namespace RenderSystem