#include "ShadowMapController.h"

#include "Viewport.h"

namespace RenderSystem
{
  ShadowMapController::ShadowMapController(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : mShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    ControllerWithDepthMap::operator=(&mShaderProgram);
  }

  void ShadowMapController::onViewportChanged(Viewport* viewport)
  {
    setDepthMapSize(viewport->getWidth(), viewport->getHeight());
  }

  void ShadowMapController::setModel(const glm::mat4& model)
  {
    mShaderProgram.setModel(model);
  }

  void ShadowMapController::setLightView(const glm::mat4& lightView)
  {
    mShaderProgram.setLightView(lightView);
  }

  void ShadowMapController::setLightProjection(const glm::mat4& lightProjection)
  {
    mShaderProgram.setLightProjection(lightProjection);
  }
}  // namespace RenderSystem
