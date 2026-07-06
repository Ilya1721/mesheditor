#include "AABBox.h"

#include "Constants.h"
#include "GeometryCore/Numeric.h"
#include "GeometryCore/Plane.h"
#include "GeometryCore/Ray.h"
#include "Mesh.h"

using namespace GeometryCore;

namespace
{
  enum class MinMaxOption
  {
    MIN,
    MAX
  };

  glm::vec3 getMinMaxPoint(
    const glm::vec3& left, const glm::vec3& right, MinMaxOption minMaxOption
  )
  {
    glm::vec3 result {};
    result.x = minMaxOption == MinMaxOption::MIN ? std::min(left.x, right.x)
                                                 : std::max(left.x, right.x);
    result.y = minMaxOption == MinMaxOption::MIN ? std::min(left.y, right.y)
                                                 : std::max(left.y, right.y);
    result.z = minMaxOption == MinMaxOption::MIN ? std::min(left.z, right.z)
                                                 : std::max(left.z, right.z);

    return result;
  }
}  // namespace

namespace MeshCore
{
  AABBox::AABBox()
  {
    clear();
    calcBBoxPlanes();
  }

  AABBox::AABBox(const glm::vec3& min, const glm::vec3& max) : mMin(min), mMax(max)
  {
    calcBBoxPlanes();
  }

  void AABBox::setMinMax(const glm::vec3& min, const glm::vec3& max)
  {
    mMin = min;
    mMax = max;
    calcBBoxPlanes();
  }

  void AABBox::applyVertices(const std::vector<Vertex>& vertices)
  {
    glm::vec3 min = mMin;
    glm::vec3 max = mMax;
    for (const auto& vertex : vertices)
    {
      min = glm::min(min, vertex.pos);
      max = glm::max(max, vertex.pos);
    }
    setMinMax(min, max);
  }

  void AABBox::clear()
  {
    constexpr auto floatMax = std::numeric_limits<float>::max();
    constexpr auto floatMin = -floatMax;
    glm::vec3 min(floatMax, floatMax, floatMax);
    glm::vec3 max(floatMin, floatMin, floatMin);
    setMinMax(min, max);
  }

  void AABBox::applyTransform(const glm::mat4& transform)
  {
    setMinMax(transform * glm::vec4(mMin, 1.0f), transform * glm::vec4(mMax, 1.0f));
  }

  void AABBox::applyBBox(const AABBox& other)
  {
    setMinMax(
      getMinMaxPoint(mMin, other.mMin, MinMaxOption::MIN),
      getMinMaxPoint(mMax, other.mMax, MinMaxOption::MAX)
    );
  }

  glm::vec3 AABBox::getCenter() const
  {
    return 0.5f * (mMin + mMax);
  }

  const glm::vec3& AABBox::getMin() const
  {
    return mMin;
  }

  const glm::vec3& AABBox::getMax() const
  {
    return mMax;
  }

  float AABBox::getHeight() const
  {
    return mMax.y - mMin.y;
  }

  float AABBox::getWidth() const
  {
    return mMax.x - mMin.x;
  }

  float AABBox::getDepth() const
  {
    return mMax.z - mMin.z;
  }

  const std::vector<Plane> AABBox::getBBoxPlanes() const
  {
    return mBBoxPlanes;
  }

  std::optional<glm::vec3> AABBox::getRayIntersectionPoint(const Ray& ray) const
  {
    for (auto& bboxPlane : mBBoxPlanes)
    {
      auto intersectionPoint = bboxPlane.getIntersectionPoint(ray);
      if (intersectionPoint && isPointInside(intersectionPoint.value()))
      {
        return intersectionPoint;
      }
    }
    return {};
  }

  bool AABBox::isPointInside(const glm::vec3& point) const
  {
    return (point.x >= mMin.x && point.x <= mMax.x) &&
           (point.y >= mMin.y && point.y <= mMax.y) &&
           (point.z >= mMin.z && point.z <= mMax.z);
  }

  bool AABBox::intersects(const AABBox& other) const
  {
    return (mMin.x <= other.mMax.x && mMax.x >= other.mMin.x) &&
           (mMin.y <= other.mMax.y && mMax.y >= other.mMin.y) &&
           (mMin.z <= other.mMax.z && mMax.z >= other.mMin.z);
  }

  void AABBox::calcBBoxPlanes()
  {
    mBBoxPlanes.clear();
    mBBoxPlanes.push_back({mMin, {-1.0f, 0.0f, 0.0f}});
    mBBoxPlanes.push_back({mMin, {0.0f, -1.0f, 0.0f}});
    mBBoxPlanes.push_back({mMin, {0.0f, 0.0f, -1.0f}});
    mBBoxPlanes.push_back({mMax, {1.0f, 0.0f, 0.0f}});
    mBBoxPlanes.push_back({mMax, {0.0f, 1.0f, 0.0f}});
    mBBoxPlanes.push_back({mMax, {0.0f, 0.0f, 1.0f}});
  }
}  // namespace MeshCore
