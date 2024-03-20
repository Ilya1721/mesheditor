#include "Plane.h"

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>

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

    glm::mat4 Plane::getPlaneToPlaneTransform(const Plane& source) const
    {
        auto rotationAxis = glm::cross(normal, source.normal);
        auto rotationAngle = glm::angle(glm::normalize(normal), glm::normalize(source.normal));
        auto rotationTransform = glm::rotate(rotationAngle, rotationAxis);
        auto translationTransform = glm::translate(origin - source.origin);
        
        return translationTransform * rotationTransform;
    }
}
