#pragma once

#include <glm/glm.hpp>

#include "Typedefs.h"

namespace MeshCore
{
  struct MaterialParams
  {
    RGB ambient;
    RGB diffuse;
    RGB specular;
    float shininess;
  };
}  // namespace MeshCore
