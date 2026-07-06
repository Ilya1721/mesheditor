#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "GeometryCore/Plane.h"
#include "Vertex.h"

namespace GeometryCore
{
  struct Ray;
}

namespace MeshCore
{
  using namespace GeometryCore;

  class Mesh;

  class AABBox
  {
   public:
    AABBox();
    AABBox(const glm::vec3& min, const glm::vec3& max);

    void applyVertices(const std::vector<Vertex>& vertices);
    void applyBBox(const AABBox& other);
    void applyTransform(const glm::mat4& transform);
    void clear();

    glm::vec3 getCenter() const;
    const glm::vec3& getMin() const;
    const glm::vec3& getMax() const;
    float getHeight() const;
    float getWidth() const;
    float getDepth() const;
    std::optional<glm::vec3> getRayIntersectionPoint(const Ray& ray) const;
    const std::vector<Plane> getBBoxPlanes() const;
    bool isPointInside(const glm::vec3& point) const;
    bool intersects(const AABBox& other) const;

   private:
    void calcBBoxPlanes();
    void setMinMax(const glm::vec3& min, const glm::vec3& max);

   private:
    glm::vec3 mMin;
    glm::vec3 mMax;
    std::vector<Plane> mBBoxPlanes;
  };
}  // namespace MeshCore
