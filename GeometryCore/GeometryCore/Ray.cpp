#include "Ray.h"

namespace GeometryCore
{
  Ray operator*(const glm::mat4& transform, const Ray& ray)
  {
    Ray transformedRay {};
    transformedRay.origin = transform * glm::vec4(ray.origin, 1.0f);
    transformedRay.direction = transform * glm::vec4(ray.direction, 0.0f);
    
    return transformedRay;
  }
}  // namespace GeometryCore