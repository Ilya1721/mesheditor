#include "Transforms.h"

#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/transform.hpp>

#include "Numeric.h"

namespace GeometryCore
{
  glm::mat4 getRotationTransform(float rotationAngle, const glm::vec3& rotationAxis)
  {
    if (isEqual(rotationAxis, glm::vec3(0.0f, 0.0f, 0.0f)))
    {
      return glm::mat4(1.0f);
    }
    return glm::rotate(rotationAngle, rotationAxis);
  }

  glm::mat4 getTranslationTransform(
    const glm::vec3& destinationPoint, const glm::vec3& sourcePoint
  )
  {
    if (isEqual(destinationPoint, sourcePoint))
    {
      return glm::mat4(1.0f);
    }
    return glm::translate(destinationPoint - sourcePoint);
  }

  glm::vec3 transformVector(const glm::vec3& vec, const glm::mat4& transform)
  {
    return transform * glm::vec4(vec, 0.0f);
  }

  glm::vec3 transformPoint(const glm::vec3& vec, const glm::mat4& transform)
  {
    return transform * glm::vec4(vec, 1.0f);
  }
}  // namespace GeometryCore