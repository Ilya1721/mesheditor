#pragma once

#include <glm/glm.hpp>

#include "BRepCurve.h"
#include "BRepModel.h"
#include "BRepSurface.h"

namespace MeshCore
{
  NURBSCurve3D createUnitXYBRepCircle();
  NURBSSurface createUnitXYBRepPlane();
  NURBSSurface createUnitBRepCylinder();
  BRepModel createBRepCylinder(
    const glm::vec3& origin, const glm::vec3& normal, float radius, float height
  );
}
