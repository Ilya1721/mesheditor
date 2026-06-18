#include "PointLightObject3D.h"

#include <glm/gtc/type_ptr.hpp>

#include "MeshCore/MeshFactory.h"
#include "PointLight.h"

using namespace MeshCore;

namespace RenderSystem
{
  PointLightObject3D::PointLightObject3D(PointLight* pointLight) : mPointLight(pointLight)
  {
    mMesh = std::make_unique<Mesh>(createUnitSphereAtOrigin());
    init();
  }

  void PointLightObject3D::updateTransform(const glm::mat4& transform)
  {
    Object3D::updateTransform(transform);
    mPointLight->setLightSourcePos(glm::value_ptr(mBBox.getCenter()));
  }
}  // namespace RenderSystem
