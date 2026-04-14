#pragma once

#include <glm/glm.hpp>

namespace GeometryCore
{
  struct Line
  {
    glm::vec3 start;
    glm::vec3 end;
    glm::mat4 getTransformToSelf(const Line& input) const;
  };
}  // namespace GeometryCore
