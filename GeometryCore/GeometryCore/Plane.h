#pragma once

#include <glm/glm.hpp>

#include "Intersectable.h"

namespace GeometryCore
{
  struct Plane : Intersectable
  {
    Plane(const Point3D& origin, const Vector3D& normal);

    Point3D origin;
    Vector3D normal;

    Point3D projectPoint(const Point3D& point) const;
    glm::mat4 getTransformToSelf(const Plane& source) const;
    std::optional<Point3D> findIntersectionPoint(const Ray& ray) const override;
  };
}  // namespace GeometryCore
