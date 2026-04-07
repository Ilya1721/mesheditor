#pragma once

#include <functional>
#include <unordered_map>

#include "HighlightedFacesData.h"

namespace RenderSystem
{
  class Object3D;
  class Renderer;

  using ObjectPrerenderSetup = std::function<void(const Object3D*)>;

  class ExtraRenderModesController
  {
   public:
    ExtraRenderModesController(
      Renderer* renderer,
      const std::unordered_map<const Object3D*, int>* objectVertexOffsetMap
    );

    void renderWireframe(const ObjectPrerenderSetup& prerenderSetup);
    void renderHighlightedFaces(const ObjectPrerenderSetup& prerenderSetup);
    void renderObjectHighlighted(const ObjectPrerenderSetup& prerenderSetup);
    void toggleWireframe();
    void setHighlightedObject(const Object3D* object);
    void setHighlightedFacesData(const HighlightedFacesData& data);

   private:
    bool mRenderWireframe;
    const Object3D* mHighlightedObject;
    HighlightedFacesData mHighlightedFacesData;
    Renderer* mRenderer;
    const std::unordered_map<const Object3D*, int>* mObjectVertexOffsetMap;
  };
}
