#pragma once

#include "Object3D.h"

namespace RenderSystem
{
  class PointLight;

  class PointLightObject3D : public Object3D
  {
   public:
    PointLightObject3D(PointLight* pointLight);

    void updateTransform(const glm::mat4& transform);

   private:
    PointLight* mPointLight;
  };
}  // namespace RenderSystem
