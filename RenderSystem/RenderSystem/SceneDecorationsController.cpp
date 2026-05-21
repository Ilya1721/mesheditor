#include "SceneDecorationsController.h"

#include "Renderer.h"
#include "ShaderProgram.h"

namespace RenderSystem
{
  SceneDecorationsController::SceneDecorationsController(Renderer* renderer)
    : mRenderer(renderer)
  {
  }

  void SceneDecorationsController::render(
    const DecorationsPrerenderSetup& prerenderSetup, const GetShader& getShader
  )
  {
    int startIndex = 0;
    for (const auto& sceneDecoration : mSceneDecorations)
    {
      prerenderSetup(sceneDecoration);
      auto shaderProgram = getShader(sceneDecoration);
      shaderProgram->invoke(
        [this, &sceneDecoration, &startIndex]()
        { mRenderer->renderSceneDecoration(sceneDecoration, startIndex); }
      );
    }
  }

  void SceneDecorationsController::addDecoration(const SceneDecoration& sceneDecoration)
  {
    mSceneDecorations.push_back(sceneDecoration);
    mSceneDecorationsRenderData.append(sceneDecoration.renderData);
    mRenderer->loadDecorationsRenderData(mSceneDecorationsRenderData);
  }
}
