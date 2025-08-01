#pragma once

#include <glm/glm.hpp>

#include "Typedefs.h"

namespace RenderSystem
{
  struct Light
  {
    RGB ambient;
    RGB diffuse;
    RGB specular;
  };
}  // namespace RenderSystem
