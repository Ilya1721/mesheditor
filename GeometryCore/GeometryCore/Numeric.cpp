#include "Numeric.h"

#include <cmath>

namespace GeometryCore
{
    bool isCloser(const glm::vec3& left, const glm::vec3& right, const glm::vec3& reference)
    {
        return glm::distance(left, reference) < glm::distance(right, reference);
    }
}