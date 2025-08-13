#pragma once

#include "Object3D.h"
#include "PointLight.h"

namespace RenderSystem
{
  class PointLightObject3D : public Object3D
  {
   public:
    PointLightObject3D(PointLight* pointLight);

   private:
    PointLight* mPointLight;
  };
}  // namespace RenderSystem
