#include "Plane.h"

#include <glm/gtx/vector_angle.hpp>

#include "Ray.h"
#include "Transforms.h"

namespace GeometryCore
{
    Plane::Plane(const Point3D& origin, const Vector3D& normal) :
        origin(origin), normal(normal)
    {}

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

        return findIntersection(ray).value();
    }

    glm::mat4 Plane::getTransformToSelf(const Plane& source) const
    {
        auto rotationAxis = glm::cross(normal, source.normal);
        auto rotationAngle = glm::angle(glm::normalize(normal), glm::normalize(source.normal));
        auto rotationTransform = getRotationTransform(rotationAngle, rotationAxis);
        auto translationTransform = getTranslationTransform(origin, source.origin);
        
        return translationTransform * rotationTransform;
    }

    std::optional<Point3D> Plane::findIntersection(const Ray& ray) const
    {
        if (glm::epsilonEqual(glm::dot(normal, ray.direction), 0.0f, 1e-6f))
        {
            return {};
        }

        auto distanceToPlane = (glm::dot(normal, origin) - glm::dot(normal, ray.origin)) / glm::dot(normal, ray.direction);

        return std::make_optional(ray.origin + distanceToPlane * ray.direction);
    }
}
