#include "Plane.h"

#include <glm/gtx/vector_angle.hpp>

#include "Ray.h"
#include "Transforms.h"

namespace GeometryCore
{
    Point3D Plane::projectPoint(const Point3D& point) const
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

    glm::mat4 Plane::getTransformToSelf(const Plane& source) const
    {
        auto rotationAxis = glm::cross(normal, source.normal);
        auto rotationAngle = glm::angle(glm::normalize(normal), glm::normalize(source.normal));
        auto rotationTransform = getRotationTransform(rotationAngle, rotationAxis);
        auto translationTransform = getTranslationTransform(origin, source.origin);
        
        return translationTransform * rotationTransform;
    }
}
