#pragma once

#include <memory>
#include <variant>

#include "ImageTexture.h"
#include "Typedefs.h"

namespace RenderSystem
{
  static inline constexpr RGB DEFAULT_RGB {1.0f, 1.0f, 1.0f};

  struct RGBOrTexture
  {
    RGB rgb = DEFAULT_RGB;
    std::shared_ptr<ImageTexture> texture;
  };

  struct BlinnPhongMaterial
  {
    RGB ambient = DEFAULT_RGB;
    RGBOrTexture diffuse;
    RGB specular = DEFAULT_RGB;
    float shininess;
  };

  struct GlassMaterial
  {
    float refractiveIndex;
    float reflectionStrength;
    float transparency;
    float interpolationFactor;
    RGB color = DEFAULT_RGB;
  };

  struct PBRMaterial
  {
    float metallic;
    RGBOrTexture roughness;
    RGBOrTexture baseColor;
    RGBOrTexture normal;
  };

  template <typename T>
  concept MaterialType = std::same_as<std::remove_cvref_t<T>, BlinnPhongMaterial> ||
                         std::same_as<std::remove_cvref_t<T>, GlassMaterial> ||
                         std::same_as<std::remove_cvref_t<T>, PBRMaterial>;

  using Material =
    std::variant<std::monostate, BlinnPhongMaterial, GlassMaterial, PBRMaterial>;
}  // namespace RenderSystem
