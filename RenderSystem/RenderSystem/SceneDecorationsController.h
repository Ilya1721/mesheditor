#pragma once

#include <functional>
#include <vector>

#include "RenderData.h"
#include "SceneDecoration.h"

namespace RenderSystem
{
  class Renderer;

  using DecorationsPrerenderSetup = std::function<void(const SceneDecoration&)>;

  class SceneDecorationsController
  {
   public:
    SceneDecorationsController(Renderer* renderer);

    void render(const DecorationsPrerenderSetup& prerenderSetup);
    void addDecoration(const SceneDecoration& sceneDecoration);

   private:
    std::vector<SceneDecoration> mSceneDecorations;
    RenderData mSceneDecorationsRenderData;
    Renderer* mRenderer;
  };
}
