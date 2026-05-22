#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "BRepCurve.h"
#include "BRepSurface.h"
#include "Vertex.h"

namespace MeshCore
{
  std::vector<Vertex> createUnitSphereAtOrigin();
  std::vector<Vertex> createUnitCubeAtOrigin();
  std::vector<Vertex> createUnitXYCircle(size_t segments);
  std::vector<Vertex> createUnitXYPlane();
  std::vector<Vertex> createUnitXLine(bool withArrowHead);
  std::vector<Vertex> getBRepCurveVertices(const NURBSCurve3D& curve, size_t segments);
  std::vector<Vertex> getBRepSurfaceVertices(
    const NURBSSurface& surface, size_t resolutionU, size_t resolutionV
  );
}  // namespace MeshCore
