#pragma once

#include <functional>
#include <memory>
#include <string>

#include "AbstractShaderProgram.h"
#include "DirLightSource.h"
#include "GeometryCore/Typedefs.h"
#include "HighlightedFacesData.h"
#include "LightParams.h"
#include "Object3D.h"
#include "Object3DIntersection.h"
#include "SceneDecoration.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  class Renderer;
  class ShadowController;
  class SceneShaderProgram;
  struct Modelable;

  class Scene
  {
   public:
    Scene(
      const std::string& meshFilePath,
      Renderer* renderer,
      ShadowController* shadowController,
      SceneShaderProgram* sceneShaderProgram,
      float aspectRatio
    );
    Scene(Scene&& scene) = delete;

    Object3DIntersection getIntersection(
      const Ray& cursorRay, IntersectionMode intersectionMode
    );
    Object3D* getPickedObject() const;
    const std::vector<SceneDecoration>& getSceneDecorations() const;
    bool getRenderWireframe() const;
    const Object3D* getHighlightedObject();
    const HighlightedFacesData& getHighlightedFacesData();
    Point3D getDefaultPointLightSourcePos() const;

    void setPickedObject(Object3D* pickedObject);
    void addSceneDecoration(const SceneDecoration& sceneDecoration);
    void toggleWireframe();
    void highlightWholeObject(const Object3D* object);
    void setHighlightedFacesData(const HighlightedFacesData& data);
    void setDirLightSourcePos(const Point3D& pos);
    void setAspectRatio(float aspectRatio);
    void addPointLight(const PointLightParams& params, const Point3D& lightSourcePos);
    void removePointLight(unsigned int index);
    void render();

    const Object3D& getRootObject() const;

   private:
    void addModelObject(const std::string& meshFilePath);
    void registerRootObjectCallbacks();
    void onObjectAddedToScene(const Object3D* object);
    void onSceneObjectUpdated(
      const Object3D* object, const std::unordered_set<UniqueVertex*>& vertices
    );
    void onSceneObjectBBoxUpdated();
    void renderScene(AbstractShaderProgram* shaderProgram);
    void renderDepthMap();
    void renderSceneDecorations();
    void renderHighlightedFaces();
    void renderWireframe();
    void renderWholeObjectHighlighted();
    void updateDirLightProjection();

   private:
    Renderer* mRenderer;
    ShadowController* mShadowController;
    SceneShaderProgram* mSceneShaderProgram;

    std::vector<SceneDecoration> mSceneDecorations;
    RenderData mSceneDecorationsRenderData;
    bool mRenderWireframe;
    const Object3D* mHighlightedObject;
    HighlightedFacesData mHighlightedFacesData;
    Object3D* mPickedObject;
    float mAspectRatio;

    RenderData mSceneRenderData;
    std::unordered_map<const Object3D*, int> mSceneObjectVertexOffsetMap;
    Object3D mRootObject;
    DirLightSource mDirLightSource;
  };
}  // namespace RenderSystem