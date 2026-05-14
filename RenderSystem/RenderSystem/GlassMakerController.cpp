#include "GlassMakerController.h"

#include "Constants.h"
#include "Object3D.h"

namespace RenderSystem
{
  void GlassMakerController::setGlassMaterial(Object3D* object)
  {
    mOriginalMaterial = object->getMaterial();
    object->setMaterial(GLASS_MATERIAL);
  }

  void GlassMakerController::resetObjectMaterial(Object3D* object) const
  {
    object->setMaterial(mOriginalMaterial);
  }
}
