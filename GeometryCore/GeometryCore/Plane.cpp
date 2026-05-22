#include "Plane.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "Ray.h"

namespace GeometryCore
{
  Plane::Plane(const glm::vec3& origin, const glm::vec3& normal)
    : origin(origin), normal(normal)
  {
  }

  glm::vec3 Plane::projectPoint(const glm::vec3& point) const
  {
    auto dotProduct = glm::dot(origin - point, normal);
    if (glm::epsilonEqual(dotProduct, 0.0f, 1e-5f))
    {
      return point;
    }
    auto rayDirection = dotProduct > 0.0f ? normal : -normal;

    return getIntersectionPoint({point, rayDirection}).value();
  }

  glm::mat4 Plane::getTransformToSelf(const Plane& source) const
  {
    auto rotationQuat = glm::quat(source.normal, normal);
    auto rotation = glm::toMat4(rotationQuat);
    auto translation = glm::translate(origin - source.origin);

    return translation * rotation;
  }

  std::optional<glm::vec3> Plane::getIntersectionPoint(const Ray& ray) const
  {
    if (glm::epsilonEqual(glm::dot(normal, ray.direction), 0.0f, 1e-6f))
    {
      return {};
    }

    auto distanceToPlane = (glm::dot(normal, origin) - glm::dot(normal, ray.origin)) /
                           glm::dot(normal, ray.direction);

    if (distanceToPlane < 0.0f)
    {
      return {};
    }

    return std::make_optional(ray.origin + ray.direction * distanceToPlane);
  }
}  // namespace GeometryCore
