#pragma once

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "Typedefs.h"

namespace GeometryCore
{
  bool isCloser(const Point3D& first, const Point3D& second, const Point3D& reference);
  float getSquareOfTriangle(const std::array<float, 3>& edgesLengths);
  double roundToDigits(double value, int digits);
  glm::vec3 roundToDigits(glm::vec3 value, int digits);

  template <typename Vector> bool isEqual(const Vector& first, const Vector& second)
  {
    return glm::all(glm::epsilonEqual(first, second, 1e-6f));
  }
}  // namespace GeometryCore