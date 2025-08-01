#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "GeometryCore/Typedefs.h"
#include "MeshCore/Intersection.h"
#include "MeshCore/Surface.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  struct RaySurfaceIntersection
  {
    MeshCore::Surface surface {};
    std::vector<int> surfaceIndices;
    Point3D point {};

    void setClosest(
      const MeshCore::MeshIntersectionData& meshIntersectionData,
      const Point3D& referencePoint
    );
  };
}  // namespace RenderSystem