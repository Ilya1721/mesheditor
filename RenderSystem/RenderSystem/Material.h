#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <variant>

#include "ImageTexture.h"

namespace RenderSystem
{
  static inline constexpr glm::vec3 DEFAULT_RGB {1.0f, 1.0f, 1.0f};

  struct RGBOrTexture
  {
    glm::vec3 rgb = DEFAULT_RGB;
    std::shared_ptr<ImageTexture> texture;
  };

  struct BlinnPhongMaterial
  {
    glm::vec3 ambient = DEFAULT_RGB;
    RGBOrTexture diffuse;
    glm::vec3 specular = DEFAULT_RGB;
    float shininess;
  };

  struct GlassMaterial
  {
    float refractiveIndex;
    float reflectionStrength;
    float transparency;
    float interpolationFactor;
    glm::vec3 color = DEFAULT_RGB;
  };

  struct PBRMaterial
  {
    std::shared_ptr<ImageTexture> baseColorTexture;
    std::shared_ptr<ImageTexture> normalMap;
    std::shared_ptr<ImageTexture> metallicRougnessTexture;
    glm::vec3 baseColor = DEFAULT_RGB;
    float metallic;
    float rougness;
  };

  template <typename T>
  concept MaterialType = std::same_as<std::remove_cvref_t<T>, BlinnPhongMaterial> ||
                         std::same_as<std::remove_cvref_t<T>, GlassMaterial> ||
                         std::same_as<std::remove_cvref_t<T>, PBRMaterial>;

  using Material =
    std::variant<std::monostate, BlinnPhongMaterial, GlassMaterial, PBRMaterial>;
}  // namespace RenderSystem
