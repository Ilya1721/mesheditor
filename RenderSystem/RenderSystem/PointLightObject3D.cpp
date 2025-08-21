#include "PointLightObject3D.h"

#include "Constants.h"
#include "MeshCore/MeshFactory.h"

using namespace MeshCore;

namespace RenderSystem
{
  PointLightObject3D::PointLightObject3D(PointLight* pointLight) : mPointLight(pointLight)
  {
    mMesh = createSphere(POINT_LIGHT_RADIUS);
    init();
  }
}  // namespace RenderSystem
