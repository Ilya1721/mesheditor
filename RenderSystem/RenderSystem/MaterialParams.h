#pragma once

#include <glm/glm.hpp>

#include "Typedefs.h"

namespace RenderSystem
{
  struct MaterialParams
  {
    RGB ambient;
    RGB diffuse;
    RGB specular;
    float shininess;
  };
}  // namespace RenderSystem
