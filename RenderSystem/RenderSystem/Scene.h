#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "AnimationController.h"
#include "BlinnPhongShaderProgram.h"
#include "Camera.h"
#include "CameraListener.h"
#include "ColorShaderProgram.h"
#include "DefaultFrameBufferObject.h"
#include "GlassShaderProgram.h"
#include "HighlightedFacesData.h"
#include "LightParams.h"
#include "MeshRenderBuffer.h"
#include "Object3D.h"
#include "Object3DIntersection.h"
#include "ObjectRenderer.h"
#include "PBRShaderProgram.h"
#include "ParticlesController.h"
#include "ParticlesRenderBuffer.h"
#include "ParticlesShaderProgram.h"
#include "PointCloudShaderProgram.h"
#include "ScreenShaderProgram.h"
#include "ShadowMapShaderProgram.h"
#include "ShadowShaderProgram.h"
#include "SkyboxShaderProgram.h"
#include "StaticQuadRenderBuffer.h"
#include "TAAController.h"
#include "TAADepthMapShaderProgram.h"
#include "TAAMotionVectorsShaderProgram.h"
#include "TAAResolveShaderProgram.h"
#include "Viewport.h"
#include "WaterController.h"
#include "WaterShaderProgram.h"

namespace RenderSystem
{
  using namespace GeometryCore;

  using ObjectHighlightRendererMap =
    std::unordered_map<const Object3D*, std::unique_ptr<ObjectHighlightRenderer>>;
  using ObjectFaceHighlightRendererMap =
    std::unordered_map<const Object3D*, std::unique_ptr<ObjectHighlightRenderer>>;

  class Scene
  {
   public:
    Scene(const Viewport* viewport, const std::string& meshFilePath);
    Scene(Scene&& scene) = delete;

    Object3DIntersection getRayIntersection(
      const Ray& cursorRay, IntersectionMode intersectionMode = IntersectionMode::SURFACE
    );
    Object3D* getPickedObject() const;
    glm::vec3 getGroundPlaneIntersection(const Ray& cursorRay, float yTranslate = 0.0f)
      const;
    glm::vec3 getDefaultPointLightSourcePos() const;
    const Object3D& getRootObject() const;
    Camera* getCamera() const;
    glm::vec3 unProject(
      const glm::vec3& posGL3D, const glm::mat4& projection, const glm::vec4& viewportData
    );

    void onViewportChanged();

    void setHighlightedObject(const Object3D* object);
    void setHighlightedFacesData(const HighlightedFacesData& data);
    void setPickedObject(Object3D* pickedObject);

    void addPointLight(const PointLightParams& params, const glm::vec3& lightSourcePos);
    void addSceneDecoration(std::unique_ptr<Object3D> decoration);

    void toggleWireframe();
    void removePointLight(unsigned int index);
    void render(float lastFrameTime);

    void updateSkinningTransforms(float lastFrameTime);
    void toggleAnimationPlaying();
    void nextAnimation();
    void prevAnimation();
    void setObjectToAnimate(Object3D* object);

    void updateParticles(float lastFrameTime);
    void startGeneratingParticles(const glm::vec3& point);
    void stopGeneratingParticles();
    void moreParticles();
    void lessParticles();

    void updateWater(float lastFrameTime);
    void startGeneratingWater(const glm::vec3& pos);
    void stopGeneratingWater();

   private:
    ParticlesRenderData getParticlesRenderData() const;
    MeshRenderData getSkyboxRenderData() const;
    MeshRenderData getWaterRenderData() const;

    void addModelObject(const std::string& meshFilePath);
    void addFloorAsObject();
    void addCameraListeners();
    void addObjectRenderers(const Object3D* object);
    void addObjectMaterialRenderers(const Object3D* object);
    void addObjectShadowRenderers(const Object3D* object);
    void addObjectTAARenderers(const Object3D* object);
    void addObjectMiscRenderers(const Object3D* object);

    void onObjectAddedToScene(const Object3D* object);
    void onSceneObjectUpdated(
      const Object3D* object, const std::unordered_set<UniqueVertex*>& vertices
    );
    void onSceneObjectBBoxUpdated();
    void onCameraChanged();

    void renderIntoShadowMapTexture() const;
    void renderIntoTAAColorTexture() const;
    void renderIntoTAADepthTexture() const;
    void renderIntoTAAMotionVectorsTexture() const;
    void renderIntoTAAResolvedColorTexture();
    void renderToScreen() const;

    void registerListenersCallbacks();
    void registerRootObjectCallbacks();

    void updateDirLightProjection();
    void setResolveShaderTextures();
    void setupRenderingSettings();
    void adjustFloor(Object3D* floor);
    void setJitteredProjectionToShaders();
    void setViewToShaders();

    void init(const std::string& meshFilePath);
    void initSceneObjects(const std::string& meshFilePath);
    void initShaders();
    void initControllers();
    void initListeners();
    void initDirLight();
    void initRenderers();
    void initRenderBuffers();
    void initTextures();

   private:
    std::unique_ptr<BlinnPhongShaderProgram> mBlinnPhongShaderProgram;
    std::unique_ptr<PBRShaderProgram> mPBRShaderProgram;
    std::unique_ptr<GlassShaderProgram> mGlassShaderProgram;
    std::unique_ptr<ShadowShaderProgram> mShadowShaderProgram;
    std::unique_ptr<ShadowMapShaderProgram> mShadowMapShaderProgram;
    std::unique_ptr<TAADepthMapShaderProgram> mTAADepthMapShaderProgram;
    std::unique_ptr<TAAMotionVectorsShaderProgram> mTAAMotionVectorsShaderProgram;
    std::unique_ptr<TAAResolveShaderProgram> mTAAResolveShaderProgram;
    std::unique_ptr<ScreenShaderProgram> mScreenShaderProgram;
    std::unique_ptr<ParticlesShaderProgram> mParticlesShaderProgram;
    std::unique_ptr<WaterShaderProgram> mWaterShaderProgram;
    std::unique_ptr<ColorShaderProgram> mColorShaderProgram;
    std::unique_ptr<SkyboxShaderProgram> mSkyboxShaderProgram;
    std::unique_ptr<PointCloudShaderProgram> mPointCloudShaderProgram;

    std::unique_ptr<TAAController> mTAAController;
    std::unique_ptr<AnimationController> mAnimationController;
    std::unique_ptr<ParticlesController> mParticlesController;
    std::unique_ptr<WaterController> mWaterController;

    std::vector<CameraListener*> mCameraListeners;
    std::unique_ptr<Camera> mCamera;
    const Viewport* mViewport;

    HighlightedFacesData mHighlightedFacesData;
    bool mRenderWireframe;

    std::shared_ptr<Texture2D> mWaterNormalMap;
    std::shared_ptr<Texture2D> mShadowMap;
    std::shared_ptr<Texture2D> mParticlesFlipbook;
    std::shared_ptr<CubemapTexture> mSkyboxTexture;

    Object3D mRootObject;
    Object3D* mPickedObject;
    const Object3D* mModelObject;
    const Object3D* mHighlightedObject;
    std::vector<std::unique_ptr<Object3D>> mDecorations;

    std::unordered_map<const Object3D*, Object3DRenderer*> mObjectRendererMap;
    ObjectHighlightRendererMap mObjectHighlightRendererMap;
    ObjectFaceHighlightRendererMap mFaceHighlightRendererMap;
    std::vector<std::unique_ptr<Object3DRenderer>> mObjectRenderers;
    std::vector<std::unique_ptr<Object3DRenderer>> mDecorationRenderers;
    std::vector<std::unique_ptr<Object3DRenderer>> mShadowMapRenderers;
    std::vector<std::unique_ptr<Object3DRenderer>> mTAADepthMapRenderers;
    std::vector<std::unique_ptr<ObjectShadowRenderer>> mShadowRenderers;
    std::vector<std::unique_ptr<ObjectWireframeRenderer>> mWireframeRenderers;
    std::vector<std::unique_ptr<ObjectMotionVectorsRenderer>> mMotionVectorsRenderers;
    std::unique_ptr<QuadRenderer> mTAAResolveRenderer;
    std::unique_ptr<QuadRenderer> mScreenRenderer;
    std::unique_ptr<SkyboxRenderer> mSkyboxRenderer;
    std::unique_ptr<Object3DRenderer> mWaterRenderer;
    std::unique_ptr<ParticlesRenderer> mParticlesRenderer;

    DefaultFrameBufferObject mScreenFrameBufferObject;
    FrameBufferObject mOffScreenFrameBufferObject;
    MeshRenderData mModelRenderData;
    MeshRenderData mDecorationsRenderData;
    MeshRenderBuffer mModelRenderBuffer;
    MeshRenderBuffer mDecorationsRenderBuffer;
    MeshRenderBuffer mSkyboxRenderBuffer;
    MeshRenderBuffer mWaterRenderBuffer;
    ParticlesRenderBuffer mParticlesRenderBuffer;
    StaticQuadRenderBuffer mScreenQuadRenderBuffer;
  };
}  // namespace RenderSystem