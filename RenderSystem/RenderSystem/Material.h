#pragma once

#include <glm/glm.hpp>

#include "Typedefs.h"

namespace RenderSystem
{
  struct Material
  {
    RGB ambient;
    RGB diffuse;
    RGB specular;
    float shininess;
  };
}  // namespace RenderSystem
