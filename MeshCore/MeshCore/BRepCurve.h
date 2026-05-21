#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace MeshCore
{
  struct CurveDomain
  {
    float min, max;
  };

  struct Curve
  {
    glm::vec4 getWeightedSum(float t) const;

    int mSmoothnessLevel {};
    std::vector<float> mBoundaries;
    std::vector<glm::vec4> mControlPoints;
    std::vector<float> mWeights;
  };

  class Curve2D : protected Curve
  {
   public:
    virtual glm::vec2 getPoint(float t) const = 0;
  };

  class Curve3D : protected Curve
  {
   public:
    virtual glm::vec3 getPoint(float t) const = 0;
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
