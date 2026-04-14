#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "AnimationController.h"
#include "BlinnPhongShaderProgram.h"
#include "Camera.h"
#include "CameraListener.h"
#include "ExtraRenderModesController.h"
#include "GlassShaderProgram.h"
#include "LightParams.h"
#include "Object3D.h"
#include "Object3DIntersection.h"
#include "PBRShaderProgram.h"
#include "Renderer.h"
#include "SceneDecorationsController.h"
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
    glm::vec3 getDefaultPointLightSourcePos() const;
    const Object3D& getRootObject() const;
    std::vector<ViewportListener*> getViewportListeners();
    Camera* getCamera() const;
    glm::vec3 unProject(
      const glm::vec3& posGL3D, const glm::mat4& projection, const glm::vec4& viewportData
    );

    void onViewportChanged(Viewport* viewport) override;

    void setHighlightedObject(const Object3D* object);
    void setHighlightedFacesData(const HighlightedFacesData& data);
    void setPickedObject(Object3D* pickedObject);

    void addPointLight(const PointLightParams& params, const glm::vec3& lightSourcePos);
    void addSceneDecorations(const std::vector<SceneDecoration>& decorations);

    void toggleWireframe();
    void removePointLight(unsigned int index);
    void render(float lastFrameTime);

    void updateSkinningTransforms(float lastFrameTime);
    void toggleAnimationPlaying();
    void nextAnimation();
    void prevAnimation();
    void setObjectToAnimate(Object3D* object);

   private:
    const TAAColorTexture& resolveTAA();

    void addModelObject(const std::string& meshFilePath);
    void addFloorAsObject();
    void addCameraListeners();

    void setBlinnPhongMaterial(const BlinnPhongMaterial& material);
    void setGlassMaterial(const GlassMaterial& material);
    void setPBRMaterial(const PBRMaterial& material);
    void setProjectionToShaders(const glm::mat4& projection);

    void writeSceneToShadowMap();
    void writeSceneToTAATextures();

    void onObjectAddedToScene(const Object3D* object);
    void onSceneObjectUpdated(
      const Object3D* object, const std::unordered_set<UniqueVertex*>& vertices
    );
    void onSceneObjectBBoxUpdated();
    void onCameraPosChanged();

    void renderSceneObjects(
      const std::function<void(const Object3D*)>& prerenderSetup, bool invokeModelShaders
    );
    void renderRawScene(
      const std::function<void(const Object3D*)>& prerenderSetup, bool invokeModelShaders
    );
    void renderFullScene(
      const std::function<void(const Object3D*)>& prerenderSetup, bool invokeModelShaders
    );
    void renderDecorations();
    void renderHighlightedFaces();
    void renderWireframe();
    void renderObjectHighlighted();
    void renderSkybox();
    void renderFinalScreenTexture(const Texture2D& texture);
    void renderShadows();

    void registerListenersCallbacks();
    void registerRootObjectCallbacks();

    void scenePrerenderSetup(const Object3D* obj);
    void updateDirLightProjection();
    void adjustFloor(Object3D* floor);

    void init(const std::string& meshFilePath);
    void initSceneObjects(const std::string& meshFilePath);
    void initShaders();
    void initControllers();
    void initListeners();
    void initDirLight();

   private:
    std::unique_ptr<Renderer> mRenderer;
    std::unique_ptr<Camera> mCamera;
    std::unique_ptr<BlinnPhongShaderProgram> mBlinnPhongShaderProgram;
    std::unique_ptr<PBRShaderProgram> mPBRShaderProgram;
    std::unique_ptr<GlassShaderProgram> mGlassShaderProgram;
    std::unique_ptr<ShadowShaderProgram> mShadowShaderProgram;
    std::unique_ptr<ScreenShaderProgram> mScreenShaderProgram;
    std::unique_ptr<ShadowMapController> mShadowMapController;
    std::unique_ptr<SkyboxController> mSkyboxController;
    std::unique_ptr<TAAController> mTAAController;
    std::unique_ptr<SceneDecorationsController> mDecorationsController;
    std::unique_ptr<ExtraRenderModesController> mExtraRenderModesController;
    std::unique_ptr<AnimationController> mAnimationController;
    std::vector<CameraListener*> mCameraListeners;

    Object3D* mModelObject;
    Object3D* mPickedObject;
    float mAspectRatio;

    RenderData mSceneRenderData;
    std::unordered_map<const Object3D*, int> mSceneObjectVertexOffsetMap;
    Object3D mRootObject;
  };
}  // namespace RenderSystem