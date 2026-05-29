#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "BRepDomain.h"

namespace MeshCore
{
  class Curve
  {
   public:
    void applyTransform(const glm::mat4& transform);

   protected:
    glm::vec4 getWeightedSum(float t) const;

   protected:
    int mSmoothnessLevel {};
    std::vector<float> mBoundaries;
    std::vector<glm::vec4> mControlPoints;
    std::vector<float> mWeights;
  };

  class Curve2D : public Curve
  {
   public:
    virtual ~Curve2D() = default;
    virtual glm::vec2 getPoint(float t) const = 0;
    virtual CurveDomain getDomain() const = 0;
  };

  class Curve3D : public Curve
  {
   public:
    virtual ~Curve3D() = default;
    virtual glm::vec3 getPoint(float t) const = 0;
  };

  class CylinderCurve2D : public Curve2D
  {
   public:
    CylinderCurve2D(float v);

    glm::vec2 getPoint(float t) const override;
    CurveDomain getDomain() const override;

   private:
    float mV;
  };

  class CircleCurve2D : public Curve2D
  {
   public:
    CircleCurve2D(float radius);

    glm::vec2 getPoint(float t) const override;
    CurveDomain getDomain() const override;

    float getRadius() const;

   private:
    float mRadius;
  };

  class NURBSCurve2D : public Curve2D
  {
   public:
    NURBSCurve2D(
      int smoothnessLevel,
      const std::vector<float>& boundaries,
      const std::vector<glm::vec4>& controlPoints,
      const std::vector<float>& weights
    );

    glm::vec2 getPoint(float t) const override;
  };

  class NURBSCurve3D : public Curve3D
  {
   public:
    NURBSCurve3D(
      int smoothnessLevel,
      const std::vector<float>& boundaries,
      const std::vector<glm::vec4>& controlPoints,
      const std::vector<float>& weights
    );

    glm::vec3 getPoint(float t) const override;
  };
}
