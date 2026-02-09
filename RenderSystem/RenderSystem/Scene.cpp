#include "Scene.h"

#include <MeshCore/TreeWalker.h>

#include <glm/gtx/transform.hpp>

#include "Constants.h"
#include "DebugVisualization.h"
#include "GeometryCore/Ray.h"
#include "MeshCore/Intersection.h"
#include "ModelLoader.h"
#include "PointLightObject3D.h"
#include "Renderer.h"
#include "SkyboxController.h"
#include "TAAController.h"
#include "Viewport.h"

using namespace GeometryCore;

namespace RenderSystem
{
  const BlinnPhongMaterial& FLOOR_MATERIAL = PEARL_MATERIAL;
  const BlinnPhongMaterial& HIGHLIGHT_MATERIAL = RUBY_MATERIAL;
  const BlinnPhongMaterial& WIREFRAME_MATERIAL = BLACK_MATERIAL;
}  // namespace RenderSystem

namespace RenderSystem
{
  Scene::Scene(const std::string& meshFilePath, float aspectRatio)
    : mPickedObject(nullptr),
      mRenderWireframe(false),
      mHighlightedObject(nullptr),
      mModelObject(nullptr),
      mAspectRatio(aspectRatio)
  {
    mRenderer = std::make_unique<Renderer>();
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
    mScreenShaderProgram = std::make_unique<ScreenShaderProgram>(
      SCREEN_VERTEX_SHADER_PATH, SCREEN_FRAGMENT_SHADER_PATH
    );
    mShadowMapController = std::make_unique<ShadowMapController>(
      SHADOW_MAP_VERTEX_SHADER_PATH, SHADOW_MAP_FRAGMENT_SHADER_PATH
    );
    mSkyboxController = std::make_unique<SkyboxController>(
      SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH, SKYBOX_CUBEMAP_TEXTURES
    );
    mTAAController = std::make_unique<TAAController>(
      TAA_DEPTH_MAP_VERTEX_SHADER_PATH, TAA_DEPTH_MAP_FRAGMENT_SHADER_PATH,
      TAA_MOTION_VECTORS_VERTEX_SHADER_PATH, TAA_MOTION_VECTORS_FRAGMENT_SHADER_PATH,
      TAA_RESOLVE_VERTEX_SHADER_PATH, TAA_RESOLVE_FRAGMENT_SHADER_PATH
    );
    mCamera = std::make_unique<Camera>();
    init(meshFilePath);
  }

  void Scene::init(const std::string& meshFilePath)
  {
    registerRootObjectCallbacks();
    addModelObject(meshFilePath);
    addFloorAsObject();
    mRenderer->loadSkyboxRenderData(RenderData::getSkyboxRenderData());
    mRenderer->loadScreenQuadRenderData();
    addCameraListeners();
    registerCallbacks();
    calcDirLightSourcePos();
    mBlinnPhongShaderProgram->setDirLightParams(DIR_LIGHT_PARAMS);
    mPBRShaderProgram->setLightColor(PBR_LIGHT_COLOR);
    mShadowShaderProgram->setShadowBias(SHADOW_BIAS);
  }

  void Scene::onCameraPosChanged()
  {
    for (auto& listener : mCameraListeners)
    {
      listener->onCameraPosChanged(mCamera.get());
    }
  }

  void Scene::registerCallbacks()
  {
    mCamera->addOnCameraPosChangedCallback([this]() { onCameraPosChanged(); });
  }

  void Scene::addCameraListeners()
  {
    mCameraListeners.insert(
      mCameraListeners.end(),
      std::initializer_list<CameraListener*> {
        mBlinnPhongShaderProgram.get(), mPBRShaderProgram.get(), mSkyboxController.get(),
        mGlassShaderProgram.get(), mShadowShaderProgram.get()
      }
    );
  }

  void Scene::renderFinalScreenTexture(const Texture2D& texture)
  {
    mScreenShaderProgram->setTexture(texture);
    mScreenShaderProgram->invoke([this]() { mRenderer->renderScreenQuad(); });
  }

  void Scene::adjustFloor(Object3D* floor)
  {
    auto yOffset = mRootObject.getBBox().getHeight();
    floor->updateTransform(glm::translate(Vector3D(0.0f, -yOffset, 0.0f)));
    floor->updateTransform(glm::scale(Vector3D(100.0f, 1.0f, 100.0f)));
    floor->setUVScale(Vector2D(10.0f, 10.0f));
  }

  void Scene::calcDirLightSourcePos()
  {
    mBlinnPhongShaderProgram->setDirLightSourcePos(DIR_LIGHT_POS);
    mPBRShaderProgram->setLightPos(DIR_LIGHT_POS);
    const auto& lightViewMatrix =
      glm::lookAt(DIR_LIGHT_POS, Point3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
    mShadowMapController->setLightView(lightViewMatrix);
    mShadowShaderProgram->setLightView(lightViewMatrix);
  }

  void Scene::setBlinnPhongMaterial(const BlinnPhongMaterial& material)
  {
    mBlinnPhongShaderProgram->setAmbient(material.ambient);
    mBlinnPhongShaderProgram->setDiffuse(material.diffuse.rgb);
    mBlinnPhongShaderProgram->setDiffuseTexture(material.diffuse.texture.get());
    mBlinnPhongShaderProgram->setSpecular(material.specular);
    mBlinnPhongShaderProgram->setShininess(material.shininess);
  }

  void Scene::setGlassMaterial(const GlassMaterial& material)
  {
    mGlassShaderProgram->setColor(material.color);
    mGlassShaderProgram->setInterpolationFactor(material.interpolationFactor);
    mGlassShaderProgram->setReflectionStrength(material.reflectionStrength);
    mGlassShaderProgram->setRefractiveIndex(material.refractiveIndex);
    mGlassShaderProgram->setTransparency(material.transparency);
    mGlassShaderProgram->setSkyboxCubemap(mSkyboxController->getCubemapTexture());
  }

  void Scene::setPBRMaterial(const PBRMaterial& material)
  {
    mPBRShaderProgram->setBaseColorTexture(*material.baseColor);
    mPBRShaderProgram->setNormalTexture(*material.normalMap);
    mPBRShaderProgram->setMetallicRoughnessTexture(*material.metallicRougness);
  }

  void Scene::setProjectionToShaders(const glm::mat4& projection)
  {
    mBlinnPhongShaderProgram->setProjection(projection);
    mPBRShaderProgram->setProjection(projection);
    mGlassShaderProgram->setProjection(projection);
    mShadowShaderProgram->setProjection(projection);
  }

  void Scene::addModelObject(const std::string& meshFilePath)
  {
    auto modelObject = loadModelFromFile(meshFilePath);
    mModelObject = modelObject.get();
    mRootObject.addChild(std::move(modelObject));
  }

  void Scene::addFloorAsObject()
  {
    auto floorObject = loadModelFromFile(FLOOR_MESH_PATH);
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
    walker.forEach([this](const Object3D* obj) {
      const auto& vertices = obj->getVertices();
      if (!vertices.empty())
      {
        mSceneObjectVertexOffsetMap.insert({ obj, mSceneRenderData.getVertexCount() });
        for (const auto& vertex : vertices) { mSceneRenderData.append(vertex); }
      }
    });
    mRenderer->loadModelRenderData(mSceneRenderData);
  }

  void Scene::onSceneObjectUpdated(
    const Object3D* object, const std::unordered_set<UniqueVertex*>& vertices
  )
  {
    for (auto& vertex : vertices)
    {
      for (auto& originalVertexData : vertex->originalVertices)
      {
        mSceneRenderData.updateVertex(
          originalVertexData, mSceneObjectVertexOffsetMap.at(object)
        );
      }
    }

    mRenderer->loadModelRenderData(mSceneRenderData);
  }

  void Scene::onSceneObjectBBoxUpdated() { updateDirLightProjection(); }

  void Scene::renderSceneObjects(
    const std::function<void(const Object3D*)>& prerenderSetup, bool invokeModelShaders
  )
  {
    auto renderBlinnPhong = std::function<void(const Object3D&, int)> {};
    auto renderGlass = std::function<void(const Object3D&, int)> {};
    auto renderPBR = std::function<void(const Object3D&, int)> {};

    if (invokeModelShaders)
    {
      renderBlinnPhong = [this](const Object3D& object, int vertexOffset)
      {
        mBlinnPhongShaderProgram->invoke(
          [this, &object, vertexOffset]()
          { mRenderer->renderBlinnPhongObject3D(object, vertexOffset); }
        );
      };

      renderGlass = [this](const Object3D& object, int vertexOffset)
      {
        mGlassShaderProgram->invoke(
          [this, &object, vertexOffset]()
          { mRenderer->renderGlassObject3D(object, vertexOffset); }
        );
      };

      renderPBR = [this](const Object3D& object, int vertexOffset)
      {
        mPBRShaderProgram->invoke([this, &object, vertexOffset]()
                                  { mRenderer->renderPBRObject3D(object, vertexOffset); }
        );
      };
    }
    else
    {
      renderBlinnPhong = [this](const Object3D& object, int vertexOffset)
      { mRenderer->renderBlinnPhongObject3D(object, vertexOffset); };
      renderGlass = [this](const Object3D& object, int vertexOffset)
      { mRenderer->renderGlassObject3D(object, vertexOffset); };
      renderPBR = [this](const Object3D& object, int vertexOffset)
      { mRenderer->renderPBRObject3D(object, vertexOffset); };
    }

    for (auto& [object, vertexOffset] : mSceneObjectVertexOffsetMap)
    {
      prerenderSetup(object);
      object->materialVisitor(
        [this, &object, vertexOffset, &renderBlinnPhong](const BlinnPhongMaterial&)
        { renderBlinnPhong(*object, vertexOffset); },
        [this, &object, vertexOffset, &renderGlass](const GlassMaterial&)
        { renderGlass(*object, vertexOffset); },
        [this, &object, vertexOffset, &renderPBR](const PBRMaterial&)
        { renderPBR(*object, vertexOffset); }
      );
    }
  }

  void Scene::renderShadows()
  {
    mShadowShaderProgram->setShadowMap(mShadowMapController->getDepthMap());
    for (auto& [object, vertexOffset] : mSceneObjectVertexOffsetMap)
    {
      mShadowShaderProgram->setModel(object->getTransform());
      mShadowShaderProgram->invoke(
        [this, &object, vertexOffset]()
        { mRenderer->renderObject3DShadow(*object, vertexOffset); }
      );
    }
  }

  void Scene::renderRawScene(
    const std::function<void(const Object3D*)>& prerenderSetup, bool invokeModelShaders
  )
  {
    renderSceneObjects(prerenderSetup, invokeModelShaders);
    mBlinnPhongShaderProgram->invoke([this]() { renderDecorations(); });
  }

  void Scene::renderFullScene(
    const std::function<void(const Object3D*)>& prerenderSetup, bool invokeModelShaders
  )
  {
    renderSkybox();
    renderRawScene(prerenderSetup, invokeModelShaders);
    renderShadows();
    mBlinnPhongShaderProgram->invoke(
      [this]()
      {
        renderHighlightedFaces();
        renderWireframe();
        renderWholeObjectHighlighted();
      }
    );
  }

  void Scene::writeSceneToShadowMap()
  {
    auto shadowMapPrerenderSetup = [this](const Object3D* obj)
    { mShadowMapController->setModel(obj->getTransform()); };
    mShadowMapController->renderSceneToDepthMap(
      [this, &shadowMapPrerenderSetup]()
      { renderRawScene(shadowMapPrerenderSetup, false); }
    );
  }

  void Scene::writeSceneToTAATextures()
  {
    auto taaPrerenderSetup = [this](const Object3D* obj)
    { mTAAController->setModel(obj, obj->getTransform()); };
    mTAAController->renderSceneToDepthMap([this, &taaPrerenderSetup]()
                                          { renderRawScene(taaPrerenderSetup, false); });
    mTAAController->renderSceneToMotionVectorsTexture(
      [this, &taaPrerenderSetup]() { renderRawScene(taaPrerenderSetup, false); }
    );
    auto scenePrerenderSetupFunc = [this](const Object3D* obj)
    { scenePrerenderSetup(obj); };
    mTAAController->renderSceneToColorBuffer(
      [this, &scenePrerenderSetupFunc]()
      { renderFullScene(scenePrerenderSetupFunc, true); }
    );
  }

  void Scene::scenePrerenderSetup(const Object3D* object)
  {
    const auto& objectModel = object->getTransform();
    object->materialVisitor(
      [this, &object, &objectModel](const BlinnPhongMaterial& material)
      {
        mBlinnPhongShaderProgram->setModel(objectModel);
        mBlinnPhongShaderProgram->setUVScale(object->getUVScale());
        setBlinnPhongMaterial(material);
      },
      [this, &object, &objectModel](const GlassMaterial& material)
      {
        mGlassShaderProgram->setModel(objectModel);
        setGlassMaterial(material);
      },
      [this, &object, &objectModel](const PBRMaterial& material)
      {
        mPBRShaderProgram->setModel(objectModel);
        mPBRShaderProgram->setUVScale(object->getUVScale());
        setPBRMaterial(material);
      }
    );
  }

  const TAAColorTexture& Scene::resolveTAA()
  {
    return mTAAController->resolveTAA([this]() { mRenderer->renderScreenQuad(); });
  }

  void Scene::renderDecorations()
  {
    mBlinnPhongShaderProgram->setModel(glm::mat4(1.0f));
    mShadowMapController->setModel(glm::mat4(1.0f));

    int startIndex = 0;
    for (const auto& sceneDecoration : mSceneDecorations)
    {
      setBlinnPhongMaterial(sceneDecoration.material);
      mRenderer->renderSceneDecoration(sceneDecoration, startIndex);
    }
  }

  void Scene::renderHighlightedFaces()
  {
    setBlinnPhongMaterial(HIGHLIGHT_MATERIAL);
    for (const auto& faceIdx : mHighlightedFacesData.facesIndices)
    {
      mBlinnPhongShaderProgram->setModel(mHighlightedFacesData.parentObject->getTransform(
      ));
      mRenderer->renderHighlightedFace(
        faceIdx, mSceneObjectVertexOffsetMap.at(mHighlightedFacesData.parentObject)
      );
    }
  }

  void Scene::renderWireframe()
  {
    if (!mRenderWireframe) { return; }

    setBlinnPhongMaterial(WIREFRAME_MATERIAL);
    for (const auto& [object, vertexOffset] : mSceneObjectVertexOffsetMap)
    {
      mBlinnPhongShaderProgram->setModel(object->getTransform());
      mRenderer->renderWireframe(object->getVertexCount());
    }
  }

  void Scene::renderWholeObjectHighlighted()
  {
    const auto& highlightedObjectIt =
      mSceneObjectVertexOffsetMap.find(mHighlightedObject);
    if (highlightedObjectIt == mSceneObjectVertexOffsetMap.end()) { return; }

    setBlinnPhongMaterial(HIGHLIGHT_MATERIAL);
    const auto& [object, vertexCount] = *highlightedObjectIt;
    mBlinnPhongShaderProgram->setModel(object->getTransform());
    mRenderer->renderWholeObjectHighlighted(*object, vertexCount);
  }

  void Scene::renderSkybox()
  {
    mSkyboxController->render([this]() { mRenderer->renderSkybox(); });
  }

  void Scene::updateDirLightProjection()
  {
    const auto& bbox = mModelObject->getBBox();
    const auto orthoSize = std::max(bbox.getWidth(), bbox.getHeight());
    const auto width = orthoSize * mAspectRatio;

    glm::mat4 lightProjectionMatrix = glm::ortho(
      -width, width, -orthoSize, orthoSize, NEAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE
    );
    mShadowShaderProgram->setLightProjection(lightProjectionMatrix);
    mShadowMapController->setLightProjection(lightProjectionMatrix);
  }

  void Scene::setPickedObject(Object3D* pickedObject) { mPickedObject = pickedObject; }

  void Scene::addSceneDecoration(const SceneDecoration& sceneDecoration)
  {
    mSceneDecorations.push_back(sceneDecoration);
    mSceneDecorationsRenderData.append(sceneDecoration.renderData);
    mRenderer->loadDecorationsRenderData(mSceneDecorationsRenderData);
  }

  void Scene::addSceneDecorations(const std::vector<SceneDecoration>& sceneDecorations)
  {
    for (const auto& sceneDecoration : sceneDecorations)
    {
      addSceneDecoration(sceneDecoration);
    }
  }

  void Scene::toggleWireframe() { mRenderWireframe = !mRenderWireframe; }

  void Scene::highlightWholeObject(const Object3D* object)
  {
    mHighlightedObject = object;
  }

  void Scene::setHighlightedFacesData(const HighlightedFacesData& data)
  {
    mHighlightedFacesData = data;
  }

  void Scene::addPointLight(const PointLightParams& params, const Point3D& lightSourcePos)
  {
    auto pointLight = mBlinnPhongShaderProgram->addPointLight(params, lightSourcePos);
    auto pointLightRadius = mModelObject->getBBox().getHeight() * 0.05f;
    auto pointLightObject3D =
      std::make_unique<PointLightObject3D>(pointLight, pointLightRadius);
    pointLightObject3D->updateTransform(glm::translate(lightSourcePos));
    mRootObject.addChild(std::move(pointLightObject3D));
  }

  void Scene::removePointLight(unsigned int index)
  {
    mBlinnPhongShaderProgram->removePointLight(index);
  }

  void Scene::render()
  {
    mRenderer->cleanScreen();
    mTAAController->setView(mCamera->getViewMatrix());
    auto projection = mTAAController->makeJitteredProjection();
    setProjectionToShaders(projection);
    writeSceneToShadowMap();
    writeSceneToTAATextures();
    const auto& taaOutputTexture = resolveTAA();
    renderFinalScreenTexture(taaOutputTexture);
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

  Object3D* Scene::getPickedObject() const { return mPickedObject; }

  Point3D Scene::getDefaultPointLightSourcePos() const
  {
    const auto& bbox = mModelObject->getBBox();
    const auto& bboxCenter = bbox.getCenter();
    const auto& bboxMax = bbox.getMax();
    auto yOffset = bboxMax.y * 0.2f;
    return {bboxCenter.x, bboxMax.y + yOffset, bboxCenter.z};
  }

  const Object3D& Scene::getRootObject() const { return mRootObject; }

  std::vector<ViewportListener*> Scene::getViewportListeners()
  {
    return {
      this, mShadowMapController.get(), mTAAController.get(), mSkyboxController.get()
    };
  }

  Point3D Scene::unProject(
    const Point3D& posGL3D, const glm::mat4& projection, const glm::vec4& viewportData
  )
  {
    return glm::unProject(posGL3D, mCamera->getViewMatrix(), projection, viewportData);
  }

  Camera* Scene::getCamera() const { return mCamera.get(); }

  void Scene::onViewportChanged(Viewport* viewport)
  {
    float width = viewport->getWidth();
    float height = viewport->getHeight();
    mAspectRatio = width / height;
    mCamera->adjust(
      viewport->getProjectionType(), mModelObject->getBBox(), viewport->getFov()
    );
  }
}  // namespace RenderSystem
