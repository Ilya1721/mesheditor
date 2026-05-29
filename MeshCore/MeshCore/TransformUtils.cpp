#include "TransformUtils.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"

namespace MeshCore
{
  glm::mat4 getUnitXYCircleTransform(
    const glm::vec3& center, const glm::vec3& normal, float radius
  )
  {
    auto scaleFactor = glm::vec3(radius, radius, 1.0f);
    auto scale = glm::scale(scaleFactor);
    auto rotationQuat = glm::quat(glm::vec3(0.0f, 0.0f, 1.0f), normal);
    auto rotation = glm::toMat4(rotationQuat);
    auto translation = glm::translate(center);

    return translation * rotation * scale;
  }

  glm::mat4 getUnitXYPlaneTransform(
    const glm::vec3& origin, const glm::vec3& normal, float width, float height
  )
  {
    GeometryCore::Plane targetPlane(origin, normal);
    GeometryCore::Plane sourcePlane(
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)
    );
    auto scaleFactor = glm::vec3(width, height, 1.0f);
    auto scale = glm::scale(scaleFactor);

    return targetPlane.getTransformToSelf(sourcePlane) * scale;
  }

  glm::mat4 getUnitXLineTransform(const glm::vec3& start, const glm::vec3& end)
  {
    GeometryCore::Line targetLine {start, end};
    GeometryCore::Line sourceLine {
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)
    };
    auto scaleFactor = glm::vec3(glm::length(end - start), 1.0f, 1.0f);
    auto scale = glm::scale(scaleFactor);

    return targetLine.getTransformToSelf(sourceLine) * scale;
  }

  glm::mat4 getUnitCylinderTransform(
    const glm::vec3& origin, const glm::vec3& normal, float radius, float height
  )
  {
    glm::vec3 scaleFactor(radius, radius, height);
    auto scale = glm::scale(scaleFactor);
    auto rotationQuat = glm::quat(glm::vec3(0.0f, 0.0f, 1.0f), normal);
    auto rotation = glm::toMat4(rotationQuat);
    auto translation = glm::translate(origin);

    return translation * rotation * scale;
  }
}
