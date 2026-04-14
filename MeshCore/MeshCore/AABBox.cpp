#include "AABBox.h"

#include "GeometryCore/Numeric.h"
#include "GeometryCore/Plane.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Transforms.h"
#include "Mesh.h"
#include "Vertex.h"

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
    init();
  }

  void AABBox::init()
  {
    clear();
    calcBBoxPlanes();
  }

  void AABBox::setMinMax(const glm::vec3& min, const glm::vec3& max)
  {
    mMin = min;
    mMax = max;
    calcBBoxPlanes();
  }

  void AABBox::applyMesh(const Mesh& mesh)
  {
    glm::vec3 min = mMin;
    glm::vec3 max = mMax;

    for (const auto& vertex : mesh.getVertices())
    {
      min.x = std::min(min.x, vertex.pos.x);
      max.x = std::max(max.x, vertex.pos.x);

      min.y = std::min(min.y, vertex.pos.y);
      max.y = std::max(max.y, vertex.pos.y);

      min.z = std::min(min.z, vertex.pos.z);
      max.z = std::max(max.z, vertex.pos.z);
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
    setMinMax(transformPoint(mMin, transform), transformPoint(mMax, transform));
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
    return (mMin + mMax) * 0.5f;
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
      if (intersectionPoint && isPointInsideBBox(intersectionPoint.value()))
      {
        return intersectionPoint;
      }
    }

    return {};
  }

  bool AABBox::isPointInsideBBox(const glm::vec3& point) const
  {
    for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
    {
      if (isLess(point[coordIdx], mMin[coordIdx]) ||
          isGreater(point[coordIdx], mMax[coordIdx]))
      {
        return false;
      }
    }

    return true;
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
