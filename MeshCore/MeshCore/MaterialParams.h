#pragma once

#include <filesystem>

#include "Typedefs.h"

namespace MeshCore
{
  struct MaterialParams
  {
    MaterialParams() = default;
    MaterialParams(
      const RGB& ambient, const RGB& diffuse, const RGB& specular, float shininess
    )
      : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
    {
    }

    RGB ambient;
    RGB diffuse;
    RGB specular;
    float shininess;
    std::filesystem::path diffuseTexturePath;
  };
}  // namespace MeshCore
