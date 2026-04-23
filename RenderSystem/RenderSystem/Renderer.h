#pragma once

#include <functional>
#include <unordered_map>

#include "MeshRenderBuffer.h"
#include "MeshRenderData.h"
#include "ParticlesRenderBuffer.h"
#include "ParticlesRenderData.h"
#include "SceneDecoration.h"
#include "StaticQuadRenderBuffer.h"

namespace RenderSystem
{
  class Object3D;

  using ObjectVertexMap = std::unordered_map<const Object3D*, int>;

  class Renderer
  {
   public:
    Renderer();
    Renderer(Renderer&&) = delete;

    void renderBlinnPhongObject3D(const Object3D& object, int vertexOffset);
    void renderPBRObject3D(const Object3D& object, int vertexOffset);
    void renderGlassObject3D(const Object3D& object, int vertexOffset);
    void renderObject3DShadow(const Object3D& object, int vertexOffset);
    void renderSceneDecoration(const SceneDecoration& sceneDecoration, int& startIndex);
    void renderHighlightedFace(int faceIdx, int vertexOffset);
    void renderWireframe(int objectVertexCount);
    void renderObjectHighlighted(const Object3D& object, int vertexOffset);
    void renderSkybox();
    void renderScreenQuad();
    void renderParticles(size_t activeParticlesCount);
    void renderWater(size_t vertexCount);
    void loadModelRenderData(const MeshRenderData& renderData);
    void loadWaterRenderData(const MeshRenderData& renderData);
    void loadDecorationsRenderData(const MeshRenderData& renderData);
    void loadSkyboxRenderData(const MeshRenderData& renderData);
    void loadParticlesRenderData();
    void loadParticleQuadRenderData(const std::vector<float>& renderData);
    void loadScreenQuadRenderData(const std::vector<float>& renderData);
    void updateParticlesRenderData(const ParticlesRenderData& renderData);
    void cleanScreen();

   private:
    void renderOverlayPrimitive(const std::function<void()>& renderFunc);

   private:
    MeshRenderBuffer mModelRenderBuffer;
    MeshRenderBuffer mDecorationsRenderBuffer;
    MeshRenderBuffer mSkyboxRenderBuffer;
    MeshRenderBuffer mWaterRenderBuffer;
    StaticQuadRenderBuffer mScreenQuadRenderBuffer;
    ParticlesRenderBuffer mParticlesRenderBuffer;
  };
}  // namespace RenderSystem
