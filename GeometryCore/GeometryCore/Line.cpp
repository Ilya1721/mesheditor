#include "Line.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace GeometryCore
{
  Line::Line(const glm::vec3& start, const glm::vec3& end) : start(start), end(end)
  {
  }

  glm::mat4 Line::getTransformToSelf(const Line& source) const
  {
    auto targetVec = end - start;
    auto sourceVec = source.end - source.start;
    auto targetDir = glm::normalize(targetVec);
    auto sourceDir = glm::normalize(sourceVec);
    auto rotationQuat = glm::quat(sourceDir, targetDir);
    auto rotation = glm::toMat4(rotationQuat);
    auto translation = glm::translate(start - source.start);

    return translation * rotation;
  }
}  // namespace GeometryCore