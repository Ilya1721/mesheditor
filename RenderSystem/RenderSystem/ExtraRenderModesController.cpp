#include "ExtraRenderModesController.h"

#include "Object3D.h"
#include "Renderer.h"

namespace RenderSystem
{
  ExtraRenderModesController::ExtraRenderModesController(
    Renderer* renderer,
    const std::unordered_map<const Object3D*, int>* objectVertexOffsetMap
  )
    : mRenderWireframe(false),
      mHighlightedObject(nullptr),
      mRenderer(renderer),
      mObjectVertexOffsetMap(objectVertexOffsetMap)
  {
  }

  void ExtraRenderModesController::renderWireframe(
    const ObjectPrerenderSetup& prerenderSetup
  )
  {
    if (!mRenderWireframe)
    {
      return;
    }

    for (const auto& [object, vertexOffset] : *mObjectVertexOffsetMap)
    {
      prerenderSetup(object);
      mRenderer->renderWireframe(object->getVertexCount());
    }
  }

  void ExtraRenderModesController::renderHighlightedFaces(
    const ObjectPrerenderSetup& prerenderSetup
  )
  {
    for (const auto& faceIdx : mHighlightedFacesData.facesIndices)
    {
      prerenderSetup(mHighlightedFacesData.parentObject);
      mRenderer->renderHighlightedFace(
        faceIdx, mObjectVertexOffsetMap->at(mHighlightedFacesData.parentObject)
      );
    }
  }

  void ExtraRenderModesController::renderObjectHighlighted(
    const ObjectPrerenderSetup& prerenderSetup
  )
  {
    const auto& highlightedObjectIt = mObjectVertexOffsetMap->find(mHighlightedObject);
    if (highlightedObjectIt == mObjectVertexOffsetMap->end())
    {
      return;
    }

    const auto& [object, vertexCount] = *highlightedObjectIt;
    prerenderSetup(object);
    mRenderer->renderObjectHighlighted(*object, vertexCount);
  }

  void ExtraRenderModesController::toggleWireframe()
  {
    mRenderWireframe = !mRenderWireframe;
  }

  void ExtraRenderModesController::setHighlightedObject(const Object3D* object)
  {
    mHighlightedObject = object;
  }

  void ExtraRenderModesController::setHighlightedFacesData(
    const HighlightedFacesData& data
  )
  {
    mHighlightedFacesData = data;
  }
}
