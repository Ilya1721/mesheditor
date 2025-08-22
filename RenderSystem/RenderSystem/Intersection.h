#pragma once

#include <vector>

#include "GeometryCore/Typedefs.h"
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
    Point3D point {};

    void assign(const MeshCore::MeshIntersection& meshIntersection);
  };
}  // namespace RenderSystem