#include "BRepUtils.h"

#include <glm/gtc/epsilon.hpp>

#include "Constants.h"

namespace MeshCore
{
  float getBasisWeight(
    size_t idx, size_t smoothnessLevel, float t, const std::vector<float>& boundaries
  )
  {
    if (smoothnessLevel == 0)
    {
      if (boundaries[idx] <= t && t < boundaries[idx + 1])
      {
        return 1.0f;
      }

      return 0.0f;
    }

    float left = 0.0f;
    float leftDenom = boundaries[idx + smoothnessLevel] - boundaries[idx];
    if (!glm::epsilonEqual(leftDenom, 0.0f, EPSILON))
    {
      left = (t - boundaries[idx]) / leftDenom *
             getBasisWeight(idx, smoothnessLevel - 1, t, boundaries);
    }

    float right = 0.0f;
    float rightDenom = boundaries[idx + smoothnessLevel + 1] - boundaries[idx + 1];
    if (!glm::epsilonEqual(rightDenom, 0.0f, EPSILON))
    {
      right = (boundaries[idx + smoothnessLevel + 1] - t) / rightDenom *
              getBasisWeight(idx + 1, smoothnessLevel - 1, t, boundaries);
    }

    return left + right;
  }

  float getBasisWeightChange(
    size_t idx, size_t smoothnessLevel, float t, const std::vector<float>& boundaries
  )
  {
    if (smoothnessLevel == 0)
    {
      return 0.0f;
    }

    float left = 0.0f;
    float leftDenom = boundaries[idx + smoothnessLevel] - boundaries[idx];
    if (!glm::epsilonEqual(leftDenom, 0.0f, EPSILON))
    {
      auto basisWeight = getBasisWeight(idx, smoothnessLevel - 1, t, boundaries);
      left = smoothnessLevel / leftDenom * basisWeight;
    }

    float right = 0.0f;
    float rightDenom = boundaries[idx + smoothnessLevel + 1] - boundaries[idx + 1];
    if (!glm::epsilonEqual(rightDenom, 0.0f, EPSILON))
    {
      auto basisWeight = getBasisWeight(idx + 1, smoothnessLevel - 1, t, boundaries);
      right = smoothnessLevel / rightDenom * basisWeight;
    }

    return left - right;
  }
}
