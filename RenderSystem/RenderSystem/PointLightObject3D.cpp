#include "PointLightObject3D.h"

#include "Constants.h"
#include "MeshCore/MeshFactory.h"

using namespace MeshCore;

namespace RenderSystem
{
  PointLightObject3D::PointLightObject3D(PointLight* pointLight, float radius) : mPointLight(pointLight)
  {
    mMesh = createSphere(radius);
    init();
  }
}  // namespace RenderSystem
