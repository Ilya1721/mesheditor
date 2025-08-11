#pragma once

#include "Typedefs.h"

namespace RenderSystem
{
  struct DirectionalLightParams
  {
    RGB ambient {};
    RGB diffuse {};
    RGB specular {};
  };

  struct PointLightParams
  {
    RGB ambient {};
    RGB diffuse {};
    RGB specular {};

    float constant {};
    float linear {};
    float quadratic {};
  };
};  // namespace RenderSystem
