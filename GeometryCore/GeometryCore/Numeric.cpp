#include "Numeric.h"

#include <cmath>
#include <numeric>

namespace GeometryCore
{
  bool isCloser(const Point3D& first, const Point3D& second, const Point3D& reference)
  {
    return glm::distance(first, reference) < glm::distance(second, reference);
  }

  float getTriangleSquare(const std::array<float, 3>& edgesLengths)
  {
    auto halfPerimeter =
      std::accumulate(edgesLengths.begin(), edgesLengths.end(), 0.0f) / 2.0f;
    return sqrtf(
      halfPerimeter * (halfPerimeter - edgesLengths[0]) *
      (halfPerimeter - edgesLengths[1]) * (halfPerimeter - edgesLengths[2])
    );
  }
}  // namespace GeometryCore