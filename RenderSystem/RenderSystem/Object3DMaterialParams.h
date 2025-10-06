#pragma once

#include "Constants.h"
#include "ImageTexture.h"
#include "MeshCore/MaterialParams.h"

namespace RenderSystem
{
  struct Object3DMaterialParams : MeshCore::MaterialParams
  {
    Object3DMaterialParams() = default;
    Object3DMaterialParams(const MeshCore::MaterialParams& materialParams)
      : MeshCore::MaterialParams(materialParams),
        diffuseTexture(materialParams.diffuseTexturePath.string(), DIFFUSE_TEXTURE_SLOT)
    {
    }

    Object3DMaterialParams& operator=(const MeshCore::MaterialParams& materialParams)
    {
      MeshCore::MaterialParams::operator=(materialParams);
      diffuseTexture = std::move(
        ImageTexture(materialParams.diffuseTexturePath.string(), DIFFUSE_TEXTURE_SLOT)
      );
      return *this;
    }

    ImageTexture diffuseTexture;
  };
}  // namespace RenderSystem
