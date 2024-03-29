#include "Numeric.h"

#include <cmath>

namespace GeometryCore
{
    bool isCloser(const glm::vec3& left, const glm::vec3& right, const glm::vec3& reference)
    {
        return glm::distance(left, reference) < glm::distance(right, reference);
    }

    float getTriangleSquare(const std::array<float, 3>& edgesLengths)
    {
        auto halfPerimeter = (edgesLengths[0] + edgesLengths[1] + edgesLengths[2]) / 2.0f;
        return sqrtf(halfPerimeter * (halfPerimeter - edgesLengths[0]) * (halfPerimeter - edgesLengths[1]) * (halfPerimeter - edgesLengths[2]));
    }
}