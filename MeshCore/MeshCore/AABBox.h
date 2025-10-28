#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "GeometryCore/Plane.h"
#include "GeometryCore/Typedefs.h"

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

    void applyMesh(const Mesh& mesh);
    void applyBBox(const AABBox& other);
    void applyTransform(const glm::mat4& transform);
    void clear();

    Point3D getCenter() const;
    const Point3D& getMin() const;
    const Point3D& getMax() const;
    float getHeight() const;
    float getWidth() const;
    float getDepth() const;
    std::optional<Point3D> getIntersectionPoint(const Ray& ray) const;
    const std::vector<Plane> getBBoxPlanes() const;
    bool isPointInsideBBox(const Point3D& point) const;

   private:
    void calcBBoxPlanes();
    void init();
    void setMinMax(const Point3D& min, const Point3D& max);

   private:
    Point3D mMin;
    Point3D mMax;
    std::vector<Plane> mBBoxPlanes;
  };
}  // namespace MeshCore
