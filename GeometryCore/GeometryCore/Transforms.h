#pragma once

#include <glm/glm.hpp>

#include "Typedefs.h"

namespace GeometryCore
{
  glm::mat4 getRotationTransform(float rotationAngle, const Vector3D& rotationAxis);
  glm::mat4 getTranslationTransform(
    const Point3D& destinationPoint, const Point3D& sourcePoint
  );
  Vector3D transformVector(const Vector3D& vec, const glm::mat4& transform);
  Point3D transformPoint(const Point3D& vec, const glm::mat4& transform);
}  // namespace GeometryCore
