#pragma once

#include "ImageTexture.h"
#include "MeshCore/MaterialParams.h"

using namespace MeshCore;

namespace RenderSystem
{
  enum class MaterialType
  {
    BLINN_PHONG,
    GLASS
  };

  struct ExtraMaterialParams
  {
    GlassMaterialParams glassMaterialParams;
  };

  struct BlinnPhongMaterialParamsExtended : BlinnPhongMaterialParams
  {
    BlinnPhongMaterialParamsExtended() = default;
    BlinnPhongMaterialParamsExtended(const BlinnPhongMaterialParams& params)
      : BlinnPhongMaterialParams(params),
        diffuseTexture(params.diffuseTexturePath.string())
    {
    }
    BlinnPhongMaterialParamsExtended& operator=(const BlinnPhongMaterialParams& params)
    {
      BlinnPhongMaterialParams::operator=(params);
      diffuseTexture = std::move(ImageTexture(params.diffuseTexturePath.string()));

      return *this;
    }

    ImageTexture diffuseTexture;
  };

  struct Object3DMaterialParams
  {
    Object3DMaterialParams() = default;
    Object3DMaterialParams(const BlinnPhongMaterialParams& params)
      : blinnPhongMaterialParams(params),
        glassMaterialParams(),
        materialType(MaterialType::BLINN_PHONG)
    {
    }

    BlinnPhongMaterialParamsExtended blinnPhongMaterialParams;
    GlassMaterialParams glassMaterialParams;
    MaterialType materialType = MaterialType::BLINN_PHONG;
  };
}  // namespace RenderSystem
