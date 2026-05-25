#include "BRepSurface.h"

#include "BRepUtils.h"

namespace
{
  float getAdjustedParameter(float p, float min, float max)
  {
    return min + p * (max - min);
  }
}

namespace MeshCore
{
  glm::vec3 NURBSSurface::getPoint(float u, float v) const
  {
    glm::vec4 numerator(0.0f, 0.0f, 0.0f, 0.0f);
    float denominator = 0.0f;

    for (int cpUIdx = 0; cpUIdx < mControlPoints.size(); ++cpUIdx)
    {
      float cpUBasisWeight = getBasisWeight(cpUIdx, mSmoothnessLevelU, u, mBoundariesU);
      for (int cpVIdx = 0; cpVIdx < mControlPoints[cpUIdx].size(); ++cpVIdx)
      {
        float cpVBasisWeight = getBasisWeight(cpVIdx, mSmoothnessLevelV, v, mBoundariesV);
        float cpWeight = mWeights[cpUIdx][cpVIdx];
        float blendingStrength = cpUBasisWeight * cpVBasisWeight * cpWeight;
        const glm::vec3& cp = mControlPoints[cpUIdx][cpVIdx];
        numerator += glm::vec4(cp * blendingStrength, blendingStrength);
        denominator += blendingStrength;
      }
    }

    return numerator / denominator;
  }

  glm::vec3 NURBSSurface::getNormal(float u, float v) const
  {
    glm::vec3 uDir(0.0f);
    glm::vec3 vDir(0.0f);

    for (size_t cpUIdx = 0; cpUIdx < mControlPoints.size(); ++cpUIdx)
    {
      float basisWeightU = getBasisWeight(cpUIdx, mSmoothnessLevelU, u, mBoundariesU);
      float basisWeightChangeU =
        getBasisWeightChange(cpUIdx, mSmoothnessLevelU, u, mBoundariesU);
      for (int cpVIdx = 0; cpVIdx < mControlPoints[cpUIdx].size(); ++cpVIdx)
      {
        float basisWeightV = getBasisWeight(cpVIdx, mSmoothnessLevelV, v, mBoundariesV);
        float basisWeightChangeV =
          getBasisWeightChange(cpVIdx, mSmoothnessLevelV, v, mBoundariesV);
        float weight = mWeights[cpUIdx][cpVIdx];
        const glm::vec3& cp = mControlPoints[cpUIdx][cpVIdx];
        float influenceChangeU = basisWeightV * basisWeightChangeU * weight;
        float influenceChangeV = basisWeightU * basisWeightChangeV * weight;
        uDir += cp * influenceChangeU;
        vDir += cp * influenceChangeV;
      }
    }

    return glm::normalize(glm::cross(uDir, vDir));
  }

  float NURBSSurface::getAdjustedU(float u) const
  {
    auto uMin = mBoundariesU[mSmoothnessLevelU];
    auto uMax = mBoundariesU[mControlPoints.size()];

    return getAdjustedParameter(u, uMin, uMax);
  }

  float NURBSSurface::getAdjustedV(float v) const
  {
    auto vMin = mBoundariesV[mSmoothnessLevelV];
    auto vMax = mBoundariesV[mControlPoints[0].size()];

    return getAdjustedParameter(v, vMin, vMax);
  }

  void NURBSSurface::setSmoothnessLevels(int smoothnessLevelU, int smoothnessLevelV)
  {
    mSmoothnessLevelU = smoothnessLevelU;
    mSmoothnessLevelV = smoothnessLevelV;
  }

  void NURBSSurface::setBoundaries(
    const std::vector<float>& boundariesU, const std::vector<float>& boundariesV
  )
  {
    mBoundariesU = boundariesU;
    mBoundariesV = boundariesV;
  }

  void NURBSSurface::setControlPoints(const std::vector<std::vector<glm::vec4>>& controlPoints)
  {
    mControlPoints = controlPoints;
  }

  void NURBSSurface::setWeights(const std::vector<std::vector<float>>& weights)
  {
    mWeights = weights;
  }
}
