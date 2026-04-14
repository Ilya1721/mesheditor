#pragma once

#include <glm/glm.hpp>
#include <optional>

#include "Ray.h"

namespace GeometryCore
{
  struct Plane
  {
    Plane(const glm::vec3& origin, const glm::vec3& normal);

    glm::vec3 origin;
    glm::vec3 normal;

    glm::vec3 projectPoint(const glm::vec3& point) const;
    glm::mat4 getTransformToSelf(const Plane& source) const;
    std::optional<glm::vec3> getIntersectionPoint(const Ray& ray) const;
  };
}  // namespace GeometryCore
