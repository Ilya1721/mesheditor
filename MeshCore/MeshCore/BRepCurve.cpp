#include "BRepCurve.h"

#include <glm/gtc/epsilon.hpp>

#include "BRepUtils.h"
#include "Constants.h"

namespace MeshCore
{
  NURBSCurve2D::NURBSCurve2D(
    int smoothnessLevel,
    const std::vector<float>& boundaries,
    const std::vector<glm::vec4>& controlPoints,
    const std::vector<float>& weights
  )
  {
    mSmoothnessLevel = smoothnessLevel;
    mBoundaries = boundaries;
    mControlPoints = controlPoints;
    mWeights = weights;
  }

  glm::vec2 NURBSCurve2D::getPoint(float t) const
  {
    glm::vec3 weightedSum = getWeightedSum(t);

    if (glm::epsilonEqual(weightedSum.z, 0.0f, EPSILON))
    {
      return glm::vec2(0.0f);
    }

    return glm::vec2(weightedSum) / weightedSum.z;
  }

  NURBSCurve3D::NURBSCurve3D(
    int smoothnessLevel,
    const std::vector<float>& boundaries,
    const std::vector<glm::vec4>& controlPoints,
    const std::vector<float>& weights
  )
  {
    mSmoothnessLevel = smoothnessLevel;
    mBoundaries = boundaries;
    mControlPoints = controlPoints;
    mWeights = weights;
  }

  glm::vec3 NURBSCurve3D::getPoint(float t) const
  {
    glm::vec4 weightedSum = getWeightedSum(t);

    if (glm::epsilonEqual(weightedSum.w, 0.0f, EPSILON))
    {
      return glm::vec3(0.0f);
    }

    return glm::vec3(weightedSum) / weightedSum.w;
  }

  glm::vec4 Curve::getWeightedSum(float t) const
  {
    glm::vec4 weightedSum(0.0f);

    for (size_t idx = 0; idx < mControlPoints.size(); ++idx)
    {
      auto basisWeight = getBasisWeight(idx, mSmoothnessLevel, t, mBoundaries);
      weightedSum += basisWeight * mWeights[idx] * mControlPoints[idx];
    }

    return weightedSum;
  }
}
