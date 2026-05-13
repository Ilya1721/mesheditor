#include "Line.h"

#include <glm/gtx/vector_angle.hpp>

namespace GeometryCore
{
  glm::mat4 Line::getTransformToSelf(const Line& input) const
  {
    auto originLineDir = end - start;
    auto inputLineDir = input.end - input.start;
    auto rotationAxis = glm::cross(originLineDir, inputLineDir);
    auto rotationAngle =
      glm::angle(glm::normalize(originLineDir), glm::normalize(inputLineDir));

    auto rotationTransform = glm::rotate(-rotationAngle, rotationAxis);
    auto translationTransform = glm::translate(start - input.start);

    return translationTransform * rotationTransform;
  }
}  // namespace GeometryCore