#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "Constants.h"
#include "GeometryCore/Typedefs.h"
#include "MeshCore/Vertex.h"
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
    Renderer(SceneShaderProgram* sceneShaderProgram);
    Renderer(Renderer&&) = delete;

    void renderObject3D(const Object3D& object, int vertexOffset = 0);
    void renderSceneDecoration(const SceneDecoration& sceneDecoration, int& startIndex);
    void renderHighlightedFace(int faceIdx, int vertexOffset);
    void renderWireframe(int objectVertexCount);
    void renderWholeObjectHighlighted(const Object3D& object, int vertexOffset);
    void loadModelRenderData(const RenderData& renderData);
    void loadDecorationsRenderData(const RenderData& renderData);
    void cleanScreen();

   private:
    void init();
    void renderOverlayPrimitive(const std::function<void()>& renderFunc);

   private:
    SceneShaderProgram* mSceneShaderProgram;
    RenderBuffer mModelRenderBuffer;
    RenderBuffer mDecorationsRenderBuffer;
  };
}  // namespace RenderSystem
