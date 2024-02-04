#include "Numeric.h"

#include <cmath>

#include "Constants.h"

namespace GeometryCore
{
    bool isEqual(double left, double right)
    {
        return std::abs(left - right) < EPSILON;
    }

    bool isCloser(const glm::vec3& left, const glm::vec3& right, const glm::vec3& reference)
    {
        return glm::distance(left, reference) < glm::distance(right, reference);
    }
}