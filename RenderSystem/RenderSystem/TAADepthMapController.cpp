#include "TAADepthMapController.h"

namespace RenderSystem
{
  TAADepthMapController::TAADepthMapController(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : mShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    ControllerWithDepthMap::operator=(&mShaderProgram);
  }

  TAADepthMapShaderProgram* TAADepthMapController::getShaderProgram()
  {
    return &mShaderProgram;
  }

  void TAADepthMapController::setModel(const glm::mat4& model)
  {
    mShaderProgram.setModel(model);
  }

  void TAADepthMapController::setView(const glm::mat4& view)
  {
    mShaderProgram.setView(view);
  }

  void TAADepthMapController::setJitteredProjection(const glm::mat4& projection)
  {
    mShaderProgram.setJitteredProjection(projection);
  }
}  // namespace RenderSystem
