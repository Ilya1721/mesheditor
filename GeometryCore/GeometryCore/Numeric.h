#pragma once

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

namespace GeometryCore
{
  bool isCloser(
    const glm::vec3& first, const glm::vec3& second, const glm::vec3& reference
  );
  float getSquareOfTriangle(const std::array<float, 3>& edgesLengths);
  double roundToDigits(double value, int digits);
  glm::vec3 roundToDigits(glm::vec3 value, int digits);

  template <typename Vector> bool isEqual(const Vector& first, const Vector& second)
  {
    return glm::all(glm::epsilonEqual(first, second, 1e-6f));
  }

  bool isGreater(float first, float second, float tolerance = 1e-6f);
  bool isLess(float first, float second, float tolerance = 1e-6f);
}  // namespace GeometryCore