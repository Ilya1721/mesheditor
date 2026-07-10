#include "Scene.h"

#include <MeshCore/TreeWalker.h>

#include <glm/gtx/transform.hpp>

#include "Constants.h"
#include "GeometryCore/Ray.h"
#include "MeshCore/Intersection.h"
#include "MeshCore/MeshFactory.h"
#include "ModelLoader.h"
#include "PointLightObject3D.h"
#include "TAAController.h"
#include "TextureFactory.h"
#include "Viewport.h"

using namespace GeometryCore;

namespace RenderSystem
{
  Scene::Scene(const Viewport* viewport, const std::string& meshFilePath)
    : mPickedObject(nullptr),
      mModelObject(nullptr),
      mHighlightedObject(nullptr),
      mRenderWireframe(false),
      mViewport(viewport)
  {
    mCamera = std::make_unique<Camera>();
    init(meshFilePath);
  }

  void Scene::init(const std::string& meshFilePath)
  {
    initTextures();
    initShaders();
    initControllers();
    initListeners();
    initSceneObjects(meshFilePath);
    initDirLight();
    setupRenderingSettings();
  }

  void Scene::initShaders()
  {
    mBlinnPhongShaderProgram = std::make_unique<BlinnPhongShaderProgram>(
      BLINN_PHONG_VERTEX_SHADER_PATH, BLINN_PHONG_FRAGMENT_SHADER_PATH
    );
    mPBRShaderProgram = std::make_unique<PBRShaderProgram>(
      PBR_VERTEX_SHADER_PATH, PBR_FRAGMENT_SHADER_PATH
    );
    mGlassShaderProgram = std::make_unique<GlassShaderProgram>(
      GLASS_VERTEX_SHADER_PATH, GLASS_FRAGMENT_SHADER_PATH
    );
    mShadowShaderProgram = std::make_unique<ShadowShaderProgram>(
      SHADOW_VERTEX_SHADER_PATH, SHADOW_FRAGMENT_SHADER_PATH
    );
    mShadowMapShaderProgram = std::make_unique<ShadowMapShaderProgram>(
      SHADOW_MAP_VERTEX_SHADER_PATH, SHADOW_MAP_FRAGMENT_SHADER_PATH
    );
    mTAADepthMapShaderProgram = std::make_unique<TAADepthMapShaderProgram>(
      TAA_DEPTH_MAP_VERTEX_SHADER_PATH, TAA_DEPTH_MAP_FRAGMENT_SHADER_PATH
    );
    mTAAMotionVectorsShaderProgram = std::make_unique<TAAMotionVectorsShaderProgram>(
      TAA_MOTION_VECTORS_VERTEX_SHADER_PATH, TAA_MOTION_VECTORS_FRAGMENT_SHADER_PATH
    );
    mTAAResolveShaderProgram = std::make_unique<TAAResolveShaderProgram>(
      TAA_RESOLVE_VERTEX_SHADER_PATH, TAA_RESOLVE_FRAGMENT_SHADER_PATH
    );
    mScreenShaderProgram = std::make_unique<ScreenShaderProgram>(
      SCREEN_VERTEX_SHADER_PATH, SCREEN_FRAGMENT_SHADER_PATH
    );
    mParticlesShaderProgram = std::make_unique<ParticlesShaderProgram>(
      PARTICLES_VERTEX_SHADER_PATH, PARTICLES_FRAGMENT_SHADER_PATH
    );
    mWaterShaderProgram = std::make_unique<WaterShaderProgram>(
      WATER_VERTEX_SHADER_PATH, WATER_FRAGMENT_SHADER_PATH
    );
    mColorShaderProgram = std::make_unique<ColorShaderProgram>(
      COLOR_VERTEX_SHADER_PATH, COLOR_FRAGMENT_SHADER_PATH
    );
    mPointCloudShaderProgram = std::make_unique<PointCloudShaderProgram>(
      POINT_CLOUD_VERTEX_SHADER_PATH, POINT_CLOUD_FRAGMENT_SHADER_PATH
    );
    mSkyboxShaderProgram = std::make_unique<SkyboxShaderProgram>(
      SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH
    );
    mGlassShaderProgram->setSkyboxCubemap(*mSkyboxTexture);
    mWaterShaderProgram->setSkyboxCubemap(*mSkyboxTexture);
    mSkyboxShaderProgram->setSkyboxCubemap(*mSkyboxTexture);
    mWaterShaderProgram->setNormalMap(*mWaterNormalMap);
    mBlinnPhongShaderProgram->setDirLightParams(DIR_LIGHT_PARAMS);
    mPBRShaderProgram->setLightColor(LIGHT_COLOR);
    mWaterShaderProgram->setLightColor(LIGHT_COLOR);
    mShadowShaderProgram->setShadowBias(SHADOW_BIAS);
    mPointCloudShaderProgram->setPointScale(CLOUD_POINT_SCALE);
    mPointCloudShaderProgram->setMinPointSize(CLOUD_POINT_MIN_SIZE);
    mPointCloudShaderProgram->setMaxPointSize(CLOUD_POINT_MAX_SIZE);
    mParticlesShaderProgram->setFlipbookTexture(*mParticlesFlipbook);
    mParticlesShaderProgram->setFlipbookRows(FLIPBOOK_ROWS);
    mParticlesShaderProgram->setFlipbookCols(FLIPBOOK_COLS);
  }

  void Scene::initControllers()
  {
    mTAAController = std::make_unique<TAAController>();
    mAnimationController = std::make_unique<AnimationController>();
    mParticlesController = std::make_unique<ParticlesController>();
    mWaterController = std::make_unique<WaterController>();
  }

  void Scene::initSceneObjects(const std::string& meshFilePath)
  {
    registerRootObjectCallbacks();
    addModelObject(meshFilePath);
    addFloorAsObject();
    initRenderBuffers();
    initRenderers();
  }

  void Scene::initListeners()
  {
    addCameraListeners();
    registerListenersCallbacks();
  }

  void Scene::onCameraChanged()
  {
    for (auto& listener : mCameraListeners)
    {
      listener->onCameraChanged(mCamera.get());
    }
  }

  void Scene::registerListenersCallbacks()
  {
    mCamera->addOnCameraChangedCallback([this]() { onCameraChanged(); });
  }

  void Scene::addCameraListeners()
  {
    mCameraListeners.insert(
      mCameraListeners.end(),
      std::initializer_list<CameraListener*> {
        mBlinnPhongShaderProgram.get(), mPBRShaderProgram.get(),
        mGlassShaderProgram.get(), mShadowShaderProgram.get(),
        mParticlesShaderProgram.get(), mWaterShaderProgram.get(),
        mColorShaderProgram.get(), mPointCloudShaderProgram.get(),
        mSkyboxShaderProgram.get(), mTAADepthMapShaderProgram.get()
      }
    );
  }

  void Scene::addObjectRenderers(const Object3D* object)
  {
    addObjectMaterialRenderers(object);
    addObjectShadowRenderers(object);
    addObjectTAARenderers(object);
    addObjectMiscRenderers(object);
  }

  void Scene::addObjectMaterialRenderers(const Object3D* object)
  {
    const auto createMaterialRenderer =
      [this, object]<typename RendererType>(const ShaderProgram* shaderProgram)
    {
      return std::make_unique<RendererType>(
        &mModelRenderBuffer, &mOffScreenFrameBufferObject, shaderProgram, object,
        mModelRenderData.getVertexCount()
      );
    };

    std::unique_ptr<Object3DRenderer> objectRenderer;
    const auto& material = object->getMaterial();
    if (dynamic_cast<const BlinnPhongMaterial*>(&material))
    {
      objectRenderer = createMaterialRenderer.operator(
      )<Object3DRenderer>(mBlinnPhongShaderProgram.get());
    }
    else if (dynamic_cast<const PBRMaterial*>(&material))
    {
      objectRenderer =
        createMaterialRenderer.operator()<Object3DRenderer>(mPBRShaderProgram.get());
    }
    else if (dynamic_cast<const GlassMaterial*>(&material))
    {
      objectRenderer =
        createMaterialRenderer.operator()<ObjectGlassRenderer>(mGlassShaderProgram.get());
    }
    else if (dynamic_cast<const PointCloudMaterial*>(&material))
    {
      objectRenderer = createMaterialRenderer.operator(
      )<Object3DRenderer>(mPointCloudShaderProgram.get());
    }

    mObjectRendererMap.insert({object, objectRenderer.get()});
    mObjectRenderers.push_back(std::move(objectRenderer));
  }

  void Scene::addObjectShadowRenderers(const Object3D* object)
  {
    const auto& material = object->getMaterial();
    if (dynamic_cast<const PointCloudMaterial*>(&material))
    {
      return;
    }

    const auto createShadowRenderer =
      [this, object]<typename RendererType>(const ShaderProgram* shaderProgram)
    {
      return std::make_unique<RendererType>(
        &mModelRenderBuffer, &mOffScreenFrameBufferObject, shaderProgram, object,
        mModelRenderData.getVertexCount()
      );
    };

    auto shadowRenderer =
      createShadowRenderer.operator()<ObjectShadowRenderer>(mShadowShaderProgram.get());
    mShadowRenderers.push_back(std::move(shadowRenderer));
    auto shadowMapRenderer =
      createShadowRenderer.operator()<Object3DRenderer>(mShadowMapShaderProgram.get());
    mShadowMapRenderers.push_back(std::move(shadowMapRenderer));
  }

  void Scene::addObjectTAARenderers(const Object3D* object)
  {
    const auto createTAARenderer =
      [this, object]<typename RendererType>(const ShaderProgram* shaderProgram)
    {
      return std::make_unique<RendererType>(
        &mModelRenderBuffer, &mOffScreenFrameBufferObject, shaderProgram, object,
        mModelRenderData.getVertexCount()
      );
    };

    auto motionVectorsRenderer = createTAARenderer.operator(
    )<ObjectMotionVectorsRenderer>(mTAAMotionVectorsShaderProgram.get());
    mMotionVectorsRenderers.push_back(std::move(motionVectorsRenderer));
    auto depthMapRenderer =
      createTAARenderer.operator()<Object3DRenderer>(mTAADepthMapShaderProgram.get());
    mTAADepthMapRenderers.push_back(std::move(depthMapRenderer));
  }

  void Scene::addObjectMiscRenderers(const Object3D* object)
  {
    const auto createMiscRenderer =
      [this, object]<typename RendererType>(const ShaderProgram* shaderProgram)
    {
      return std::make_unique<RendererType>(
        &mModelRenderBuffer, &mOffScreenFrameBufferObject, shaderProgram, object,
        mModelRenderData.getVertexCount()
      );
    };

    auto wireframeRenderer = createMiscRenderer.operator(
    )<ObjectWireframeRenderer>(mBlinnPhongShaderProgram.get());
    mWireframeRenderers.push_back(std::move(wireframeRenderer));
    auto highlightRenderer = createMiscRenderer.operator(
    )<ObjectHighlightRenderer>(mBlinnPhongShaderProgram.get());
    mObjectHighlightRendererMap.insert({object, std::move(highlightRenderer)});
    auto faceHighlightRenderer = createMiscRenderer.operator(
    )<ObjectHighlightRenderer>(mBlinnPhongShaderProgram.get());
    mFaceHighlightRendererMap.insert({object, std::move(faceHighlightRenderer)});
  }

  void Scene::addSceneDecoration(std::unique_ptr<Object3D> decoration)
  {
    auto object = decoration.get();
    auto vertexOffset = mDecorationsRenderData.getVertexCount();
    const auto createObjectRenderer =
      [this, object, vertexOffset](const ShaderProgram* shaderProgram)
    {
      return std::make_unique<Object3DRenderer>(
        &mDecorationsRenderBuffer, &mOffScreenFrameBufferObject, shaderProgram, object,
        vertexOffset
      );
    };

    const auto& material = decoration->getMaterial();
    std::unique_ptr<Object3DRenderer> renderer;
    if (dynamic_cast<const BlinnPhongMaterial*>(&material))
    {
      renderer = createObjectRenderer(mBlinnPhongShaderProgram.get());
    }
    else if (dynamic_cast<const ColorMaterial*>(&material))
    {
      renderer = createObjectRenderer(mColorShaderProgram.get());
    }
    mDecorationRenderers.push_back(std::move(renderer));
    mDecorationsRenderData.append(decoration->getVertices());
    mDecorationsRenderBuffer.loadRenderData(mDecorationsRenderData);
    mDecorations.push_back(std::move(decoration));
  }

  void Scene::adjustFloor(Object3D* floor)
  {
    auto halfBBoxHeight = mRootObject.getBBox().getHeight() * 0.5;
    floor->updateTransform(glm::translate(glm::vec3(0.0f, -halfBBoxHeight, 0.0f)));
    floor->updateTransform(glm::scale(glm::vec3(100.0f, 1.0f, 100.0f)));
    floor->setUVScale(glm::vec2(10.0f, 10.0f));
  }

  void Scene::initDirLight()
  {
    mBlinnPhongShaderProgram->setDirLightPos(DIR_LIGHT_POS);
    mPBRShaderProgram->setLightPos(DIR_LIGHT_POS);
    mPointCloudShaderProgram->setLightPos(DIR_LIGHT_POS);
    mWaterShaderProgram->setLightPos(DIR_LIGHT_POS);
    const auto& lightViewMatrix = glm::lookAt(
      DIR_LIGHT_POS, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
    );
    mShadowMapShaderProgram->setLightView(lightViewMatrix);
    mShadowShaderProgram->setLightView(lightViewMatrix);
  }

  MeshRenderData Scene::getSkyboxRenderData() const
  {
    const auto& vertices = MeshCore::createUnitCubeAtOrigin();
    return MeshRenderData::generateRenderData(vertices);
  }

  MeshRenderData Scene::getWaterRenderData() const
  {
    const auto& vertices = mWaterController->getWaterPlane()->getVertices();
    return MeshRenderData::generateRenderData(vertices);
  }

  void Scene::initRenderBuffers()
  {
    mSkyboxRenderBuffer.loadRenderData(getSkyboxRenderData());
    mScreenQuadRenderBuffer.loadRenderData(SCREEN_QUAD_VERTICES);
    mParticlesRenderBuffer.loadQuadRenderData(PARTICLE_QUAD_VERTICES);
    mParticlesRenderBuffer.loadParticlesRenderData();
    mWaterRenderBuffer.loadRenderData(getWaterRenderData());
  }

  void Scene::initTextures()
  {
    mSkyboxTexture = std::make_shared<CubemapTexture>(SKYBOX_CUBEMAP_TEXTURES);
    mWaterNormalMap = createImageTexture(WATER_NORMAL_MAP_PATH);
    mParticlesFlipbook = createImageTexture(FIRE_FLIPBOOK_PATH);
  }

  void Scene::initRenderers()
  {
    mTAAResolveRenderer = std::make_unique<QuadRenderer>(
      &mScreenQuadRenderBuffer, &mOffScreenFrameBufferObject,
      mTAAResolveShaderProgram.get()
    );
    mScreenRenderer = std::make_unique<QuadRenderer>(
      &mScreenQuadRenderBuffer, &mScreenFrameBufferObject, mScreenShaderProgram.get()
    );
    mSkyboxRenderer = std::make_unique<SkyboxRenderer>(
      &mSkyboxRenderBuffer, &mOffScreenFrameBufferObject, mSkyboxShaderProgram.get()
    );
    mWaterRenderer = std::make_unique<Object3DRenderer>(
      &mWaterRenderBuffer, &mOffScreenFrameBufferObject, mWaterShaderProgram.get(),
      mWaterController->getWaterPlane(), 0
    );
    mParticlesRenderer = std::make_unique<ParticlesRenderer>(
      &mParticlesRenderBuffer, &mOffScreenFrameBufferObject,
      mParticlesShaderProgram.get(), &mParticlesController->getActiveParticlesIndices()
    );
  }

  void Scene::updateSkinningTransforms(float lastFrameTime)
  {
    mAnimationController->updateSkinningTransforms(lastFrameTime);
    const auto& skinningTransforms = mAnimationController->getSkinningTransforms();
    mPBRShaderProgram->setSkinningTransforms(skinningTransforms);
    mShadowMapShaderProgram->setSkinningTransforms(skinningTransforms);
    mShadowShaderProgram->setSkinningTransforms(skinningTransforms);
  }

  void Scene::toggleAnimationPlaying()
  {
    mAnimationController->toggleAnimationPlaying();
  }

  void Scene::nextAnimation()
  {
    mAnimationController->nextAnimation();
  }

  void Scene::prevAnimation()
  {
    mAnimationController->prevAnimation();
  }

  void Scene::setObjectToAnimate(Object3D* object)
  {
    mAnimationController->setObjectToAnimate(object);
    auto useSkinningTransforms = mAnimationController->useSkinningTransforms();
    mPBRShaderProgram->setUseSkinningTransform(useSkinningTransforms);
    mShadowMapShaderProgram->setUseSkinningTransform(useSkinningTransforms);
    mShadowShaderProgram->setUseSkinningTransform(useSkinningTransforms);
  }

  void Scene::updateParticles(float lastFrameTime)
  {
    if (mParticlesController->isGeneratingParticles())
    {
      mParticlesController->update(lastFrameTime);
      auto renderData = getParticlesRenderData();
      mParticlesRenderBuffer.updateParticlesRenderData(renderData);
    }
  }

  void Scene::startGeneratingParticles(const glm::vec3& point)
  {
    mParticlesController->startGeneratingParticles(point);
  }

  void Scene::stopGeneratingParticles()
  {
    mParticlesController->stopGeneratingParticles();
  }

  void Scene::moreParticles()
  {
    mParticlesController->moreParticles();
  }

  void Scene::lessParticles()
  {
    mParticlesController->lessParticles();
  }

  void Scene::updateWater(float lastFrameTime)
  {
    if (mWaterController->isGeneratingWater())
    {
      mWaterController->updateWater(lastFrameTime);
      auto currentTime = mWaterController->getCurrentTime();
      mWaterShaderProgram->setVTime(currentTime);
      mWaterShaderProgram->setFTime(currentTime);
    }
  }

  void Scene::startGeneratingWater(const glm::vec3& pos)
  {
    mWaterController->startGeneratingWater();
    auto model = glm::translate(glm::mat4(1.0f), pos);
    mWaterController->updateWaterPlaneTransform(model);
  }

  void Scene::stopGeneratingWater()
  {
    mWaterController->stopGeneratingWater();
  }

  void Scene::setJitteredProjectionToShaders()
  {
    auto projection = mTAAController->getJitteredProjection();
    mBlinnPhongShaderProgram->setProjection(projection);
    mPBRShaderProgram->setProjection(projection);
    mGlassShaderProgram->setProjection(projection);
    mShadowShaderProgram->setProjection(projection);
    mColorShaderProgram->setProjection(projection);
    mPointCloudShaderProgram->setProjection(projection);
    mTAADepthMapShaderProgram->setProjection(projection);
    mTAAMotionVectorsShaderProgram->setProjection(projection);
    mParticlesShaderProgram->setProjection(projection);
    mSkyboxShaderProgram->setProjection(projection);
    mWaterShaderProgram->setProjection(projection);
  }

  void Scene::setViewToShaders()
  {
    mTAAMotionVectorsShaderProgram->setView(mCamera->getViewMatrix());
  }

  void Scene::addModelObject(const std::string& meshFilePath)
  {
    auto modelObject = loadModel(meshFilePath);
    mModelObject = modelObject.get();
    mRootObject.addChild(std::move(modelObject));
  }

  void Scene::addFloorAsObject()
  {
    auto floorObject = loadModel(FLOOR_MESH_PATH);
    adjustFloor(floorObject.get());
    mRootObject.addChild(std::move(floorObject));
  }

  void Scene::registerRootObjectCallbacks()
  {
    mRootObject.addOnChildAddedCallback([this](const Object3D* object)
                                        { onObjectAddedToScene(object); });
    mRootObject.addOnObjectUpdatedCallback(
      [this](const Object3D* object, const std::unordered_set<UniqueVertex*>& vertices)
      { onSceneObjectUpdated(object, vertices); }
    );
    mRootObject.addOnBBoxUpdatedCallback([this]() { onSceneObjectBBoxUpdated(); });
  }

  void Scene::onObjectAddedToScene(const Object3D* object)
  {
    TreeWalker walker(object, true);
    walker.forEach(
      [this](const Object3D* obj)
      {
        const auto& vertices = obj->getVertices();
        if (!vertices.empty())
        {
          addObjectRenderers(obj);
          mModelRenderData.append(vertices);
        }
      }
    );
    mModelRenderBuffer.loadRenderData(mModelRenderData);
  }

  void Scene::onSceneObjectUpdated(
    const Object3D* object, const std::unordered_set<UniqueVertex*>& vertices
  )
  {
    for (auto& vertex : vertices)
    {
      for (auto& originalVertexData : vertex->originalVertices)
      {
        mModelRenderData.updateVertex(
          originalVertexData, mObjectRendererMap.at(object)->getVertexOffset()
        );
      }
    }
    mModelRenderBuffer.loadRenderData(mModelRenderData);
  }

  void Scene::onSceneObjectBBoxUpdated()
  {
    updateDirLightProjection();
  }

  void Scene::renderIntoTAAColorTexture() const
  {
    auto colorTexture = mTAAController->getCurrColorTexture();
    mOffScreenFrameBufferObject.attachTexture(
      *colorTexture, GL_COLOR_ATTACHMENT0, GL_NONE, GL_COLOR_ATTACHMENT0, true
    );
    mSkyboxRenderer->render();
    for (const auto& renderer : mObjectRenderers)
    {
      renderer->render();
    }
    for (const auto& renderer : mDecorationRenderers)
    {
      renderer->render();
    }
    for (const auto& shadowRenderer : mShadowRenderers)
    {
      shadowRenderer->render();
    }

    if (mWaterController->isGeneratingWater())
    {
      mWaterRenderer->render();
    }
    if (mParticlesController->isGeneratingParticles())
    {
      mParticlesRenderer->render();
    }

    if (mRenderWireframe)
    {
      for (const auto& renderer : mWireframeRenderers)
      {
        renderer->render();
      }
    }
    if (mHighlightedObject)
    {
      mObjectHighlightRendererMap.at(mHighlightedObject)->render();
    }
    if (mHighlightedFacesData.parentObject)
    {
      auto object = mHighlightedFacesData.parentObject;
      for (const auto& faceIdx : mHighlightedFacesData.facesIndices)
      {
        mFaceHighlightRendererMap.at(object)->renderFace(faceIdx);
      }
    }
  }

  void Scene::renderIntoTAADepthTexture() const
  {
    auto depthTexture = mTAAController->getCurrDepthTexture();
    mOffScreenFrameBufferObject.attachTexture(
      *depthTexture, GL_DEPTH_ATTACHMENT, GL_NONE, GL_NONE, false
    );
    for (const auto& renderer : mTAADepthMapRenderers)
    {
      renderer->render();
    }
  }

  void Scene::renderIntoTAAMotionVectorsTexture() const
  {
    auto motionVectorsTexture = mTAAController->getMotionVectorsTexture();
    mOffScreenFrameBufferObject.attachTexture(
      *motionVectorsTexture, GL_COLOR_ATTACHMENT0, GL_NONE, GL_COLOR_ATTACHMENT0, true
    );
    for (const auto& renderer : mMotionVectorsRenderers)
    {
      renderer->render();
    }
  }

  void Scene::renderIntoShadowMapTexture() const
  {
    mOffScreenFrameBufferObject.attachTexture(
      *mShadowMap, GL_DEPTH_ATTACHMENT, GL_NONE, GL_NONE, false
    );
    for (const auto& renderer : mShadowMapRenderers)
    {
      renderer->render();
    }
  }

  void Scene::renderIntoTAAResolvedColorTexture()
  {
    auto resolvedColorTexture = mTAAController->getResolvedColorTexture();
    mOffScreenFrameBufferObject.attachTexture(
      *resolvedColorTexture, GL_COLOR_ATTACHMENT0, GL_NONE, GL_COLOR_ATTACHMENT0, true
    );
    mTAAResolveRenderer->render();
    mTAAController->swapTextures();
    mTAAResolveShaderProgram->setIsFirstFrame(false);
  }

  void Scene::renderToScreen() const
  {
    mScreenRenderer->render();
  }

  ParticlesRenderData Scene::getParticlesRenderData() const
  {
    const auto& activeParticlesIndices =
      mParticlesController->getActiveParticlesIndices();
    const auto& particles = mParticlesController->getParticles();
    ParticlesRenderData renderData;
    for (const auto& particleIdx : activeParticlesIndices)
    {
      renderData.append(particles[particleIdx]);
    }

    return renderData;
  }

  void Scene::updateDirLightProjection()
  {
    const auto& bbox = mModelObject->getBBox();
    const auto orthoSize = std::max(bbox.getWidth(), bbox.getHeight());
    const auto width = orthoSize * mViewport->getAspectRatio();
    glm::mat4 lightProjectionMatrix = glm::ortho(
      -width, width, -orthoSize, orthoSize, NEAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE
    );
    mShadowMapShaderProgram->setLightProjection(lightProjectionMatrix);
    mShadowShaderProgram->setLightProjection(lightProjectionMatrix);
  }

  void Scene::setupRenderingSettings()
  {
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
  }

  void Scene::setResolveShaderTextures()
  {
    mTAAResolveShaderProgram->setPrevDepthMap(*mTAAController->getPrevDepthTexture());
    mTAAResolveShaderProgram->setCurrDepthMap(*mTAAController->getCurrDepthTexture());
    mTAAResolveShaderProgram->setPrevColorTexture(*mTAAController->getPrevColorTexture());
    mTAAResolveShaderProgram->setCurrColorTexture(*mTAAController->getCurrColorTexture());
    mTAAResolveShaderProgram->setMotionVectorsTexture(
      *mTAAController->getMotionVectorsTexture()
    );
  }

  void Scene::setPickedObject(Object3D* pickedObject)
  {
    mPickedObject = pickedObject;
  }

  void Scene::toggleWireframe()
  {
    mRenderWireframe = !mRenderWireframe;
  }

  void Scene::setHighlightedObject(const Object3D* object)
  {
    mHighlightedObject = object;
  }

  void Scene::setHighlightedFacesData(const HighlightedFacesData& data)
  {
    mHighlightedFacesData = data;
  }

  void Scene::addPointLight(
    const PointLightParams& params, const glm::vec3& lightSourcePos
  )
  {
    auto pointLight = mBlinnPhongShaderProgram->addPointLight(params, lightSourcePos);
    auto pointLightRadius = mModelObject->getBBox().getHeight() * 0.05f;
    auto pointLightObject3D = std::make_unique<PointLightObject3D>(pointLight);
    glm::vec3 scaleVec(pointLightRadius, pointLightRadius, pointLightRadius);
    pointLightObject3D->updateTransform(
      glm::translate(lightSourcePos) * glm::scale(scaleVec)
    );
    mRootObject.addChild(std::move(pointLightObject3D));
  }

  void Scene::removePointLight(unsigned int index)
  {
    mBlinnPhongShaderProgram->removePointLight(index);
  }

  void Scene::render(float lastFrameTime)
  {
    updateSkinningTransforms(lastFrameTime);
    updateParticles(lastFrameTime);
    updateWater(lastFrameTime);
    setJitteredProjectionToShaders();
    setViewToShaders();
    setResolveShaderTextures();
    renderIntoShadowMapTexture();
    renderIntoTAADepthTexture();
    renderIntoTAAMotionVectorsTexture();
    renderIntoTAAColorTexture();
    renderIntoTAAResolvedColorTexture();
    renderToScreen();
  }

  Object3DIntersection Scene::getRayIntersection(
    const Ray& cursorRay, IntersectionMode intersectionMode
  )
  {
    if (mRootObject.getBBox().getRayIntersectionPoint(cursorRay))
    {
      return mRootObject.getRayIntersection(cursorRay, intersectionMode);
    }

    return {};
  }

  Object3D* Scene::getPickedObject() const
  {
    return mPickedObject;
  }

  glm::vec3 Scene::getGroundPlaneIntersection(const Ray& cursorRay, float yTranslate)
    const
  {
    auto halfBBoxHeight = mRootObject.getBBox().getHeight() * 0.5;
    glm::vec3 origin(0.0f, -halfBBoxHeight + yTranslate, 0.0f);
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);
    Plane groundPlane(origin, upVector);
    auto intersectionPoint = groundPlane.getIntersectionPoint(cursorRay);

    return intersectionPoint.value_or(origin);
  }

  glm::vec3 Scene::getDefaultPointLightSourcePos() const
  {
    const auto& bbox = mModelObject->getBBox();
    const auto& bboxCenter = bbox.getCenter();
    const auto& bboxMax = bbox.getMax();
    auto yOffset = bboxMax.y * 0.2f;
    return {bboxCenter.x, bboxMax.y + yOffset, bboxCenter.z};
  }

  const Object3D& Scene::getRootObject() const
  {
    return mRootObject;
  }

  glm::vec3 Scene::unProject(
    const glm::vec3& posGL3D, const glm::mat4& projection, const glm::vec4& viewportData
  )
  {
    return glm::unProject(posGL3D, mCamera->getViewMatrix(), projection, viewportData);
  }

  Camera* Scene::getCamera() const
  {
    return mCamera.get();
  }

  void Scene::onViewportChanged()
  {
    auto width = mViewport->getWidth();
    auto height = mViewport->getHeight();
    mCamera->adjust(
      mViewport->getProjectionType(), mModelObject->getBBox(), mViewport->getFov()
    );
    mShadowMap = createDepthTexture(width, height);
    mShadowShaderProgram->setShadowMap(*mShadowMap);
    mTAAController->update(mViewport->getProjectionMatrix(), width, height);
    mScreenShaderProgram->setScreenTexture(*mTAAController->getResolvedColorTexture());
    mTAAResolveShaderProgram->setScreenSize(glm::vec2(width, height));
  }
}  // namespace RenderSystem
