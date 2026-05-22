#pragma once

#include <glm/glm.hpp>

namespace GeometryCore
{
  struct Line
  {
    Line(const glm::vec3& start, const glm::vec3& end);

    glm::vec3 start;
    glm::vec3 end;

    glm::mat4 getTransformToSelf(const Line& source) const;
  };
}  // namespace GeometryCore
