#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "BlinnPhongShaderProgram.h"
#include "Camera.h"
#include "CameraListener.h"
#include "GeometryCore/Typedefs.h"
#include "GlassShaderProgram.h"
#include "HighlightedFacesData.h"
#include "LightParams.h"
#include "Object3D.h"
#include "Object3DIntersection.h"
#include "Renderer.h"
#include "SceneDecoration.h"
#include "ScreenShaderProgram.h"
#include "ShadowMapController.h"
#include "ShadowShaderProgram.h"
#include "SkyboxController.h"
#include "TAAController.h"
#include "ViewportListener.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  struct Modelable;

  class Scene : public ViewportListener
  {
   public:
    Scene(const std::string& meshFilePath, float aspectRatio);
    Scene(Scene&& scene) = delete;

    Object3DIntersection getRayIntersection(
      const Ray& cursorRay, IntersectionMode intersectionMode
    );
    Object3D* getPickedObject() const;
    Point3D getDefaultPointLightSourcePos() const;
    const Object3D& getRootObject() const;
    std::vector<ViewportListener*> getViewportListeners();
    Camera* getCamera() const;
    Point3D unProject(
      const Point3D& posGL3D, const glm::mat4& projection, const glm::vec4& viewportData
    );

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

   private:
    void addModelObject(const std::string& meshFilePath);
    void addFloorAsObject();
    void registerRootObjectCallbacks();
    void onObjectAddedToScene(const Object3D* object);
    void onSceneObjectUpdated(
      const Object3D* object, const std::unordered_set<UniqueVertex*>& vertices
    );
    void onSceneObjectBBoxUpdated();
    void renderSceneObjects(
      const std::function<void(const Object3D*)>& prerenderSetup, bool invokeModelShaders
    );
    void renderRawScene(
      const std::function<void(const Object3D*)>& prerenderSetup, bool invokeModelShaders
    );
    void renderFullScene(
      const std::function<void(const Object3D*)>& prerenderSetup, bool invokeModelShaders
    );
    void writeSceneToShadowMap();
    void writeSceneToTAATextures();
    void scenePrerenderSetup(const Object3D* obj);
    const TAAColorTexture& resolveTAA();
    void renderDecorations();
    void renderHighlightedFaces();
    void renderWireframe();
    void renderWholeObjectHighlighted();
    void renderSkybox();
    void updateDirLightProjection();
    void init(const std::string& meshFilePath);
    void onCameraPosChanged();
    void registerCallbacks();
    void addCameraListeners();
    void renderFinalScreenTexture(const Texture2D& texture);
    void adjustFloor(Object3D* floor);
    void calcDirLightSourcePos();
    void setBlinnPhongMaterial(const BlinnPhongMaterial& material);
    void setGlassMaterial(const GlassMaterial& material);
    void setPBRMaterial(const PBRMaterial& material);

   private:
    std::unique_ptr<Renderer> mRenderer;
    std::unique_ptr<BlinnPhongShaderProgram> mBlinnPhongShaderProgram;
    std::unique_ptr<GlassShaderProgram> mGlassShaderProgram;
    std::unique_ptr<ShadowShaderProgram> mShadowShaderProgram;
    std::unique_ptr<ScreenShaderProgram> mScreenShaderProgram;
    std::unique_ptr<Camera> mCamera;
    std::unique_ptr<ShadowMapController> mShadowMapController;
    std::unique_ptr<SkyboxController> mSkyboxController;
    std::unique_ptr<TAAController> mTAAController;
    std::vector<CameraListener*> mCameraListeners;

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
    Object3D* mModelObject;
  };
}  // namespace RenderSystem