#pragma once

#include "MaterialParams.h"

namespace MeshCore
{
  constexpr bool SMOOTHING_ENABLED = false;
  constexpr float ARROW_HEAD_ANGLE = 30.0f;
  constexpr float ARROW_HEAD_LENGTH_COEF = 0.15f;

  const MaterialParams GOLD_MATERIAL {
    RGB(0.24725f, 0.1995f, 0.0745f), RGB(0.75164f, 0.60648f, 0.22648f),
    RGB(0.628281f, 0.555802f, 0.366065f), 2.0f
  };
}  // namespace MeshCore
