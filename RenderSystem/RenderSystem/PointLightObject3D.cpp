#include "PointLightObject3D.h"

#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "MeshCore/MeshFactory.h"

using namespace MeshCore;

namespace RenderSystem
{
  PointLightObject3D::PointLightObject3D(PointLight* pointLight, float radius)
    : mPointLight(pointLight)
  {
    mMesh = createSphere(radius);
    init();
  }

  void PointLightObject3D::updateTransform(const glm::mat4& transform)
  {
    Object3D::updateTransform(transform);
    mPointLight->setLightSourcePos(glm::value_ptr(mBBox.getCenter()));
  }
}  // namespace RenderSystem
