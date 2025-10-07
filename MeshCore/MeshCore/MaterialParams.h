#pragma once

#include <filesystem>

#include "Typedefs.h"

namespace MeshCore
{
  struct BlinnPhongMaterialParams
  {
    BlinnPhongMaterialParams() = default;
    BlinnPhongMaterialParams(
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

  struct GlassMaterialParams
  {
    float refractiveIndex;
    float reflectionStrength;
    float transparency;
    float interpolationFactor;
    RGB color;
  };
}
