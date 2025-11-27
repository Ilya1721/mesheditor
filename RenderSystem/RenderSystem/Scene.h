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
#include "ViewportListener.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  class Renderer;
  class ShadowController;
  class SkyboxController;
  class TAAController;
  class SceneShaderProgram;
  struct Modelable;

  class Scene : public ViewportListener
  {
   public:
    Scene(
      const std::string& meshFilePath,
      Renderer* renderer,
      ShadowController* shadowController,
      SkyboxController* skyboxController,
      TAAController* taaController,
      SceneShaderProgram* sceneShaderProgram,
      float aspectRatio
    );
    Scene(Scene&& scene) = delete;

    Object3DIntersection getRayIntersection(
      const Ray& cursorRay, IntersectionMode intersectionMode
    );
    Object3D* getPickedObject() const;
    const std::vector<SceneDecoration>& getSceneDecorations() const;
    bool getRenderWireframe() const;
    const Object3D* getHighlightedObject();
    const HighlightedFacesData& getHighlightedFacesData();
    Point3D getDefaultPointLightSourcePos() const;

    void onViewportChanged(Viewport* viewport) override;

    void setPickedObject(Object3D* pickedObject);
    void addSceneDecoration(const SceneDecoration& sceneDecoration);
    void addSceneDecorations(const std::vector<SceneDecoration>& sceneDecorations);
    void toggleWireframe();
    void highlightWholeObject(const Object3D* object);
    void setHighlightedFacesData(const HighlightedFacesData& data);
    void addPointLight(const PointLightParams& params, const Point3D& lightSourcePos);
    void removePointLight(unsigned int index);
    void render();
    void adjustDirLightSourcePos();

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
    void writeSceneToTextures();
    void renderSceneDecorations();
    void renderHighlightedFaces();
    void renderWireframe();
    void renderWholeObjectHighlighted();
    void renderSkybox();
    void updateDirLightProjection();
    void loadSkyboxVertices();
    void prepareTAA();
    void postAdjustTAA();

   private:
    Renderer* mRenderer;
    ShadowController* mShadowController;
    SkyboxController* mSkyboxController;
    TAAController* mTAAController;
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