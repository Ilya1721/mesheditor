#include "BRepSurface.h"

#include "BRepUtils.h"

namespace MeshCore
{
  glm::vec3 BRepNURBSSurface::getPoint(float u, float v)
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

  glm::vec3 BRepNURBSSurface::getNormal(float u, float v)
  {
    glm::vec3 uDir(0.0f);
    glm::vec3 vDir(0.0f);

    for (int cpUIdx = 0; cpUIdx < mControlPoints.size(); ++cpUIdx)
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
}
