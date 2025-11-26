#include "ShadowController.h"

namespace RenderSystem
{
  ShadowController::ShadowController(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : mShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    ControllerWithDepthMap::operator=(&mShaderProgram);
  }

  void ShadowController::setModel(const glm::mat4& model)
  {
    mShaderProgram.setModel(model);
  }

  void ShadowController::setLightView(const glm::mat4& lightView)
  {
    mShaderProgram.setLightView(lightView);
  }

  void ShadowController::setLightProjection(const glm::mat4& lightProjection)
  {
    mShaderProgram.setLightProjection(lightProjection);
  }

  ShadowMapShaderProgram* ShadowController::getShaderProgram() { return &mShaderProgram; }
}  // namespace RenderSystem
