#pragma once

#include <functional>
#include <unordered_map>

#include "RenderBuffer.h"
#include "SceneDecoration.h"

namespace RenderSystem
{
  class Object3D;
  class SceneShaderProgram;

  using ObjectVertexMap = std::unordered_map<const Object3D*, int>;

  class Renderer
  {
   public:
    Renderer();
    Renderer(Renderer&&) = delete;

    void renderObject3D(const Object3D& object, int vertexOffset = 0);
    void renderSceneDecoration(const SceneDecoration& sceneDecoration, int& startIndex);
    void renderHighlightedFace(int faceIdx, int vertexOffset);
    void renderWireframe(int objectVertexCount);
    void renderWholeObjectHighlighted(const Object3D& object, int vertexOffset);
    void renderSkybox();
    void renderScreenQuad();
    void loadModelRenderData(const RenderData& renderData);
    void loadDecorationsRenderData(const RenderData& renderData);
    void loadSkyboxRenderData(const RenderData& renderData);
    void loadScreenQuadRenderData();
    void cleanScreen();

   private:
    void renderOverlayPrimitive(const std::function<void()>& renderFunc);
    void renderBlinnPhongObject3D(const Object3D& object, int vertexOffset);
    void renderGlassObject3D(const Object3D& object, int vertexOffset);

   private:
    RenderBuffer mModelRenderBuffer;
    RenderBuffer mDecorationsRenderBuffer;
    RenderBuffer mSkyboxRenderBuffer;
    RenderBuffer mScreenQuadRenderBuffer;
  };
}  // namespace RenderSystem
