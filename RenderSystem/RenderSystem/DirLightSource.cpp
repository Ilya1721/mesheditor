#include "DirLightSource.h"

#include <glm/ext/matrix_transform.hpp>

#include "SceneShaderProgram.h"
#include "ShadowController.h"

namespace RenderSystem
{
  DirLightSource::DirLightSource(
    SceneShaderProgram* sceneShaderProgram, ShadowController* shadowController
  )
    : mPos(), mSceneShaderProgram(sceneShaderProgram), mShadowController(shadowController)
  {
  }

  const Point3D& DirLightSource::getPosition() const { return mPos; }

  void DirLightSource::setPosition(const Point3D& pos)
  {
    mPos = pos;
    mSceneShaderProgram->setDirLightSourcePos(pos);

    const auto& lightViewMatrix =
      glm::lookAt(pos, Point3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
    mShadowController->getShaderProgram()->setLightView(lightViewMatrix);
    mSceneShaderProgram->setLightView(lightViewMatrix);
  }
}  // namespace RenderSystem
