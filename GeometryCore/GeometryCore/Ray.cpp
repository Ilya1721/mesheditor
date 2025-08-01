#include "Ray.h"
#include "Transforms.h"

namespace GeometryCore
{
  Ray operator*(const glm::mat4& transform, const Ray& ray)
  {
    return {
      transformPoint(ray.origin, transform), transformVector(ray.direction, transform)
    };
  }
}  // namespace GeometryCore