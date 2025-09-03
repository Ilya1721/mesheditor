#include "Numeric.h"

#include <cmath>
#include <numeric>

namespace GeometryCore
{
  bool isCloser(const Point3D& first, const Point3D& second, const Point3D& reference)
  {
    return glm::distance(first, reference) < glm::distance(second, reference);
  }

  float getSquareOfTriangle(const std::array<float, 3>& edgesLengths)
  {
    auto halfPerimeter =
      std::accumulate(edgesLengths.begin(), edgesLengths.end(), 0.0f) * 0.5f;
    return sqrtf(
      halfPerimeter * (halfPerimeter - edgesLengths[0]) *
      (halfPerimeter - edgesLengths[1]) * (halfPerimeter - edgesLengths[2])
    );
  }

  double roundToDigits(double value, int digits)
  {
    double factor = std::pow(10.0, digits);
    return std::round(value * factor) / factor;
  }

  glm::vec3 roundToDigits(glm::vec3 value, int digits)
  {
    float factor = std::pow(10.0f, digits);
    return glm::round(value * factor) / factor;
  }
}  // namespace GeometryCore