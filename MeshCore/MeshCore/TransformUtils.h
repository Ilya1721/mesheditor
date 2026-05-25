#pragma once

#include <glm/glm.hpp>

namespace MeshCore
{
  glm::mat4 getUnitXYCircleTransform(
    const glm::vec3& center, const glm::vec3& normal, float radius
  );
  glm::mat4 getUnitXYPlaneTransform(
    const glm::vec3& origin, const glm::vec3& normal, float width, float height
  );
  glm::mat4 getUnitXLineTransform(const glm::vec3& start, const glm::vec3& end);
  glm::mat4 getUnitCylinderTransform(const glm::vec3& origin, const glm::vec3& normal, float radius, float height);
}
