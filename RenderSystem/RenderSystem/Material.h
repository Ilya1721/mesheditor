#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Texture2D.h"

namespace RenderSystem
{
  struct Material
  {
    virtual ~Material() = default;
    virtual void setUVScale(const glm::vec2& uvScale) {};
    virtual std::shared_ptr<Material> clone() const
    {
      return std::make_shared<Material>(*this);
    };
  };

  struct BlinnPhongMaterial : public Material
  {
    BlinnPhongMaterial() = default;

    BlinnPhongMaterial(
      const glm::vec3& ambientIn,
      const glm::vec3& diffuseIn,
      const glm::vec3& specularIn,
      float shininessIn,
      const std::shared_ptr<Texture2D>& diffuseTextureIn = nullptr
    )
      : ambient(ambientIn),
        diffuse(diffuseIn),
        specular(specularIn),
        shininess(shininessIn),
        diffuseTexture(diffuseTextureIn)
    {
    }

    glm::vec3 ambient {};
    glm::vec3 diffuse {};
    glm::vec3 specular {};
    std::shared_ptr<Texture2D> diffuseTexture;
    float shininess {};
    glm::vec2 uvScale = glm::vec2(1.0f);

    void setUVScale(const glm::vec2& newUVScale) override
    {
      uvScale = newUVScale;
    }

    std::shared_ptr<Material> clone() const override
    {
      return std::make_shared<BlinnPhongMaterial>(*this);
    };
  };

  struct GlassMaterial : public Material
  {
    GlassMaterial(
      float refractiveIndexIn,
      float reflectionStrengthIn,
      float transparencyIn,
      float interpolationFactorIn,
      const glm::vec3& colorIn
    )
      : refractiveIndex(refractiveIndexIn),
        reflectionStrength(reflectionStrengthIn),
        transparency(transparencyIn),
        interpolationFactor(interpolationFactorIn),
        color(colorIn)
    {
    }

    float refractiveIndex {};
    float reflectionStrength {};
    float transparency {};
    float interpolationFactor {};
    glm::vec3 color;

    std::shared_ptr<Material> clone() const override
    {
      return std::make_shared<GlassMaterial>(*this);
    };
  };

  struct PBRMaterial : public Material
  {
    std::shared_ptr<Texture2D> baseColorTexture;
    std::shared_ptr<Texture2D> normalMap;
    std::shared_ptr<Texture2D> metallicRougnessTexture;
    glm::vec3 baseColor;
    float metallic {};
    float rougness {};
    glm::vec2 uvScale = glm::vec2(1.0f);

    void setUVScale(const glm::vec2& newUVScale) override
    {
      uvScale = newUVScale;
    }

    std::shared_ptr<Material> clone() const override
    {
      return std::make_shared<PBRMaterial>(*this);
    };
  };

  struct ColorMaterial : public Material
  {
    glm::vec3 color;

    std::shared_ptr<Material> clone() const override
    {
      return std::make_shared<ColorMaterial>(*this);
    };
  };

  struct PointCloudMaterial : public Material
  {
    std::shared_ptr<Material> clone() const override
    {
      return std::make_shared<PointCloudMaterial>(*this);
    };
  };

  struct Wave
  {
    float amplitude {};
    float length {};
    float speed {};
    glm::vec2 direction;
  };

  struct WaterMaterial : Material
  {
    int fresnelPower {};
    float depthFalloff {};
    float reflectionIntensity {};
    float normalStrength {};
    glm::vec3 shallowColor;
    glm::vec3 deepColor;
    std::vector<glm::vec2> normalMapMoves;
    std::vector<Wave> waves;

    virtual std::shared_ptr<Material> clone() const override
    {
      return std::make_shared<WaterMaterial>(*this);
    };
  };
}  // namespace RenderSystem
