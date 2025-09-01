#pragma once

#include "ImageTexture.h"
#include "MeshCore/MaterialParams.h"

namespace RenderSystem
{
  struct Object3DMaterialParams : MeshCore::MaterialParams
  {
    Object3DMaterialParams() = default;
    Object3DMaterialParams(const MeshCore::MaterialParams& materialParams)
      : MeshCore::MaterialParams(materialParams),
        diffuseTexture(materialParams.diffuseTexturePath.string())
    {
    }
    ImageTexture diffuseTexture;
  };
}  // namespace RenderSystem
