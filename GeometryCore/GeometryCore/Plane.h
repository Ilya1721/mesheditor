#pragma once

#include <glm/glm.hpp>
#include <optional>

#include "Ray.h"
#include "Typedefs.h"

namespace GeometryCore
{
  struct Plane
  {
    Plane(const Point3D& origin, const Vector3D& normal);

    Point3D origin;
    Vector3D normal;

    Point3D projectPoint(const Point3D& point) const;
    glm::mat4 getTransformToSelf(const Plane& source) const;
    std::optional<Point3D> getIntersectionPoint(const Ray& ray) const;
  };
}  // namespace GeometryCore
