#pragma once

#include "Material.h"

namespace RenderSystem
{
  class Object3D;

  class GlassMakerController
  {
  public:
    void setGlassMaterial(Object3D* object);
    void resetObjectMaterial(Object3D* object) const;

  private:
    Material mOriginalMaterial;
  };
}
