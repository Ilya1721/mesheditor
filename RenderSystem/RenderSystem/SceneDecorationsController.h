#pragma once

#include <functional>
#include <vector>

#include "MeshRenderData.h"
#include "SceneDecoration.h"

namespace RenderSystem
{
  class Renderer;
  class ShaderProgram;

  using DecorationsPrerenderSetup = std::function<void(const SceneDecoration&)>;
  using GetShader = std::function<ShaderProgram*(const SceneDecoration&)>;

  class SceneDecorationsController
  {
   public:
    SceneDecorationsController(Renderer* renderer);

    void render(
      const DecorationsPrerenderSetup& prerenderSetup, const GetShader& getShader
    );
    void addDecoration(const SceneDecoration& sceneDecoration);

   private:
    std::vector<SceneDecoration> mSceneDecorations;
    MeshRenderData mSceneDecorationsRenderData;
    Renderer* mRenderer;
  };
}
