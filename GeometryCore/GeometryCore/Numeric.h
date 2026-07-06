#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

namespace GeometryCore
{
  bool isCloser(
    const glm::vec3& first, const glm::vec3& second, const glm::vec3& reference
  );
  double roundToDigits(double value, int digits);
  glm::vec3 roundToDigits(const glm::vec3& value, int digits);
  float getTriangleArea(const glm::vec3& leftEdge, const glm::vec3& rightEdge);

  template <typename Vector> bool isEqual(const Vector& first, const Vector& second)
  {
    return glm::all(glm::epsilonEqual(first, second, 1e-6f));
  }
}  // namespace GeometryCore