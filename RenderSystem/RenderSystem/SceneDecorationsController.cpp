#include "SceneDecorationsController.h"

#include "Renderer.h"

namespace RenderSystem
{
  SceneDecorationsController::SceneDecorationsController(Renderer* renderer)
    : mRenderer(renderer)
  {
  }

  void SceneDecorationsController::render(const DecorationsPrerenderSetup& prerenderSetup)
  {
    int startIndex = 0;
    for (const auto& sceneDecoration : mSceneDecorations)
    {
      prerenderSetup(sceneDecoration);
      mRenderer->renderSceneDecoration(sceneDecoration, startIndex);
    }
  }

  void SceneDecorationsController::addDecoration(const SceneDecoration& sceneDecoration)
  {
    mSceneDecorations.push_back(sceneDecoration);
    mSceneDecorationsRenderData.append(sceneDecoration.renderData);
    mRenderer->loadDecorationsRenderData(mSceneDecorationsRenderData);
  }
}
