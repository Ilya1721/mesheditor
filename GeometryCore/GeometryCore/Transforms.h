#pragma once

#include <glm/glm.hpp>

namespace GeometryCore
{
  glm::mat4 getRotationTransform(float rotationAngle, const glm::vec3& rotationAxis);
  glm::mat4 getTranslationTransform(
    const glm::vec3& destinationPoint, const glm::vec3& sourcePoint
  );
  glm::vec3 transformVector(const glm::vec3& vec, const glm::mat4& transform);
  glm::vec3 transformPoint(const glm::vec3& vec, const glm::mat4& transform);
}  // namespace GeometryCore
