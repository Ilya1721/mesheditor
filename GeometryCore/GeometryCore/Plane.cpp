#include "Plane.h"

#include <glm/gtc/epsilon.hpp>

#include "Ray.h"

namespace GeometryCore
{
    glm::vec3 Plane::projectPoint(const glm::vec3& point) const
    {
        auto pointToOriginVec = origin - point;
        auto dotProduct = glm::dot(pointToOriginVec, normal);

        if (glm::epsilonEqual(dotProduct, 0.0f, 1e-5f))
        {
            return point;
        }
        
        auto rayDirection = dotProduct > 0.0f ? normal : -normal;
        Ray ray{ point, rayDirection };

        return ray.findIntersection(*this).value();
    }
}
