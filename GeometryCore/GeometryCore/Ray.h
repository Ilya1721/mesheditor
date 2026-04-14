#pragma once

#include <glm/glm.hpp>

namespace GeometryCore
{
  struct Ray
  {
    glm::vec3 origin;
    glm::vec3 direction;

    friend Ray operator*(const glm::mat4& transform, const Ray& ray);
  };
}  // namespace GeometryCore
