#include "Numeric.h"

#include <cmath>
#include <numeric>

namespace GeometryCore
{
  bool isCloser(
    const glm::vec3& first, const glm::vec3& second, const glm::vec3& reference
  )
  {
    return glm::distance(first, reference) < glm::distance(second, reference);
  }

  double roundToDigits(double value, int digits)
  {
    double factor = std::pow(10.0, digits);
    return std::round(value * factor) / factor;
  }

  glm::vec3 roundToDigits(const glm::vec3& value, int digits)
  {
    float factor = std::pow(10.0f, digits);
    return glm::round(value * factor) / factor;
  }

  float getTriangleArea(const glm::vec3& leftEdge, const glm::vec3& rightEdge)
  {
    auto crossProductVector = glm::cross(leftEdge, rightEdge);
    return glm::length(crossProductVector) * 0.5f;
  }

  bool isGreater(float first, float second, float tolerance)
  {
    return first - second > tolerance;
  }

  bool isLess(float first, float second, float tolerance)
  {
    return second - first > tolerance;
  }
}  // namespace GeometryCore