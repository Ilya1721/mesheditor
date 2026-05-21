#pragma once

#include <vector>

namespace MeshCore
{
  float getBasisWeight(
    size_t idx, size_t smoothnessLevel, float t, const std::vector<float>& boundaries
  );

  float getBasisWeightChange(
    size_t idx, size_t smoothnessLevel, float t, const std::vector<float>& boundaries
  );
}
