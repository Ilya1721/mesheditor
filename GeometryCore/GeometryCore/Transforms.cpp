#include "Numeric.h"
#include "Transforms.h"
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/transform.hpp>

namespace GeometryCore
{
  glm::mat4 getRotationTransform(float rotationAngle, const Vector3D& rotationAxis)
  {
    if (isEqual(rotationAxis, Vector3D(0.0f, 0.0f, 0.0f))) { return glm::mat4(1.0f); }
    return glm::rotate(rotationAngle, rotationAxis);
  }

  glm::mat4 getTranslationTransform(
    const Point3D& destinationPoint, const Point3D& sourcePoint
  )
  {
    if (isEqual(destinationPoint, sourcePoint)) { return glm::mat4(1.0f); }
    return glm::translate(destinationPoint - sourcePoint);
  }

  Vector3D transformVector(const Vector3D& vec, const glm::mat4& transform)
  {
    return transform * Vector4D(vec, 0.0f);
  }

  Point3D transformPoint(const Point3D& vec, const glm::mat4& transform)
  {
    return transform * Point4D(vec, 1.0f);
  }
}  // namespace GeometryCore