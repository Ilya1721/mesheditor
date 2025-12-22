#include "Scene.h"

#include <glm/gtx/transform.hpp>

#include "Constants.h"
#include "DebugVisualization.h"
#include "GeometryCore/Ray.h"
#include "MeshCore/Intersection.h"
#include "MeshFilesLoader/MeshFilesLoader.h"
#include "PointLightObject3D.h"
#include "Renderer.h"
#include "SceneShaderProgram.h"
#include "ShadowController.h"
#include "SkyboxController.h"
#include "TAAController.h"
#include "Viewport.h"

using namespace GeometryCore;

namespace RenderSystem
{
  const BlinnPhongMaterialParams FLOOR_MATERIAL = PEARL_MATERIAL;
  const BlinnPhongMaterialParams HIGHLIGHT_MATERIAL = RUBY_MATERIAL;
  const BlinnPhongMaterialParams WIREFRAME_MATERIAL = BLACK_MATERIAL;
}  // namespace RenderSystem

namespace RenderSystem
{
  Scene::Scene(const std::string& meshFilePath, float aspectRatio)
    : mPickedObject(nullptr),
      mRenderWireframe(false),
      mHighlightedObject(nullptr),
      mDirLightSource(),
      mAspectRatio(aspectRatio)
  {
    mRenderer = std::make_unique<Renderer>();
    mSceneShaderProgram = std::make_unique<SceneShaderProgram>(
      SCENE_VERTEX_SHADER_PATH, SCENE_FRAGMENT_SHADER_PATH
    );
    mScreenShaderProgram = std::make_unique<ScreenShaderProgram>(
      SCREEN_VERTEX_SHADER_PATH, SCREEN_FRAGMENT_SHADER_PATH
    );
    mShadowController = std::make_unique<ShadowController>(
      SHADOW_MAP_VERTEX_SHADER_PATH, SHADOW_MAP_FRAGMENT_SHADER_PATH
    );
    mSkyboxController = std::make_unique<SkyboxController>(
      SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH, SKYBOX_CUBEMAP_TEXTURES,
      mSceneShaderProgram.get()
    );
    mTAAController = std::make_unique<TAAController>(
      mSceneShaderProgram.get(), TAA_DEPTH_MAP_VERTEX_SHADER_PATH,
      TAA_DEPTH_MAP_FRAGMENT_SHADER_PATH, TAA_MOTION_VECTORS_VERTEX_SHADER_PATH,
      TAA_MOTION_VECTORS_FRAGMENT_SHADER_PATH, TAA_RESOLVE_VERTEX_SHADER_PATH,
      TAA_RESOLVE_FRAGMENT_SHADER_PATH
    );
    mCamera = std::make_unique<Camera>();
    mDirLightSource = DirLightSource(mSceneShaderProgram.get(), mShadowController.get());
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
    mDirLightSource.setPosition(DIR_LIGHT_POS);
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
        mSceneShaderProgram.get(), mSkyboxController.get()
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
  }

  void Scene::addModelObject(const std::string& meshFilePath)
  {
    auto modelObject =
      std::make_unique<Object3D>(MeshFilesLoader::loadMeshFromFile(meshFilePath));
    mRootObject.addChild(std::move(modelObject));
  }

  void Scene::addFloorAsObject()
  {
    auto floorObject =
      std::make_unique<Object3D>(MeshFilesLoader::loadMeshFromFile(FLOOR_MESH_PATH));
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
    mSceneObjectVertexOffsetMap.insert({object, mSceneRenderData.getVertexCount()});
    for (const auto& vertex : object->getVertices()) { mSceneRenderData.append(vertex); }
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
    const std::function<void(const Object3D*)>& prerenderSetup
  )
  {
    for (auto& [object, vertexOffset] : mSceneObjectVertexOffsetMap)
    {
      prerenderSetup(object);
      mRenderer->renderObject3D(*object, vertexOffset);
    }
  }

  void Scene::renderRawScene(const std::function<void(const Object3D*)>& prerenderSetup)
  {
    renderSceneObjects(prerenderSetup);
    renderDecorations();
  }

  void Scene::renderFullScene(const std::function<void(const Object3D*)>& prerenderSetup)
  {
    renderSkybox();
    renderRawScene(prerenderSetup);
    renderHighlightedFaces();
    renderWireframe();
    renderWholeObjectHighlighted();
  }

  void Scene::writeSceneToShadowMap()
  {
    auto shadowMapPrerenderSetup = [this](const Object3D* obj)
    { mShadowController->setModel(obj->getTransform()); };
    mShadowController->renderSceneToDepthMap([this, &shadowMapPrerenderSetup]()
                                             { renderRawScene(shadowMapPrerenderSetup); }
    );
  }

  void Scene::writeSceneToTAATextures()
  {
    auto taaPrerenderSetup = [this](const Object3D* obj)
    { mTAAController->setModel(obj, obj->getTransform()); };
    mTAAController->renderSceneToDepthMap([this, &taaPrerenderSetup]()
                                          { renderRawScene(taaPrerenderSetup); });
    mTAAController->renderSceneToMotionVectorsTexture(
      [this, &taaPrerenderSetup]() { renderRawScene(taaPrerenderSetup); }
    );
    auto scenePrerenderSetupFunc = [this](const Object3D* obj)
    { scenePrerenderSetup(obj); };
    mTAAController->renderSceneToColorBuffer([this, &scenePrerenderSetupFunc]()
                                             { renderFullScene(scenePrerenderSetupFunc); }
    );
  }

  void Scene::scenePrerenderSetup(const Object3D* obj)
  {
    const auto& materialParams = obj->getMaterialParams();
    mSceneShaderProgram->setModel(obj->getTransform());
    mSceneShaderProgram->setBlinnPhongMaterialParams(
      materialParams.blinnPhongMaterialParams
    );
    mSceneShaderProgram->setGlassMaterialParams(materialParams.glassMaterialParams);
    mSceneShaderProgram->setMaterialType(materialParams.materialType);
    mSceneShaderProgram->setShadowMap(mShadowController->getDepthMap());
  }

  const TAAColorTexture& Scene::resolveTAA()
  {
    return mTAAController->resolveTAA([this]() { mRenderer->renderScreenQuad(); });
  }

  void Scene::renderDecorations()
  {
    mSceneShaderProgram->setModel(glm::mat4(1.0f));
    mShadowController->setModel(glm::mat4(1.0f));

    int startIndex = 0;
    for (const auto& sceneDecoration : mSceneDecorations)
    {
      mSceneShaderProgram->setBlinnPhongMaterialParams(sceneDecoration.materialParams);
      mRenderer->renderSceneDecoration(sceneDecoration, startIndex);
    }
  }

  void Scene::renderHighlightedFaces()
  {
    mSceneShaderProgram->setBlinnPhongMaterialParams(HIGHLIGHT_MATERIAL);
    for (const auto& faceIdx : mHighlightedFacesData.facesIndices)
    {
      mSceneShaderProgram->setModel(mHighlightedFacesData.parentObject->getTransform());
      mRenderer->renderHighlightedFace(
        faceIdx, mSceneObjectVertexOffsetMap.at(mHighlightedFacesData.parentObject)
      );
    }
  }

  void Scene::renderWireframe()
  {
    if (!mRenderWireframe) { return; }

    mSceneShaderProgram->setBlinnPhongMaterialParams(WIREFRAME_MATERIAL);
    for (const auto& [object, vertexOffset] : mSceneObjectVertexOffsetMap)
    {
      mSceneShaderProgram->setModel(object->getTransform());
      mRenderer->renderWireframe(object->getVertexCount());
    }
  }

  void Scene::renderWholeObjectHighlighted()
  {
    const auto& highlightedObjectIt =
      mSceneObjectVertexOffsetMap.find(mHighlightedObject);
    if (highlightedObjectIt == mSceneObjectVertexOffsetMap.end()) { return; }

    mSceneShaderProgram->setBlinnPhongMaterialParams(HIGHLIGHT_MATERIAL);
    const auto& [object, vertexCount] = *highlightedObjectIt;
    mSceneShaderProgram->setModel(object->getTransform());
    mRenderer->renderWholeObjectHighlighted(*object, vertexCount);
  }

  void Scene::renderSkybox()
  {
    mSkyboxController->render([this]() { mRenderer->renderSkybox(); });
  }

  void Scene::updateDirLightProjection()
  {
    const auto& bbox = mRootObject.getBBox();
    const auto orthoSize = std::max(bbox.getWidth(), bbox.getHeight());
    const auto width = orthoSize * mAspectRatio;

    glm::mat4 lightProjectionMatrix = glm::ortho(
      -width, width, -orthoSize, orthoSize, NEAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE
    );
    mSceneShaderProgram->setLightProjection(lightProjectionMatrix);
    mShadowController->setLightProjection(lightProjectionMatrix);
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
    auto pointLight = mSceneShaderProgram->addPointLight(params, lightSourcePos);
    auto pointLightRadius = mRootObject.getBBox().getHeight() * 0.05f;
    auto pointLightObject3D =
      std::make_unique<PointLightObject3D>(pointLight, pointLightRadius);
    pointLightObject3D->updateTransform(glm::translate(lightSourcePos));
    mRootObject.addChild(std::move(pointLightObject3D));
  }

  void Scene::removePointLight(unsigned int index)
  {
    mSceneShaderProgram->removePointLight(index);
  }

  void Scene::render()
  {
    mRenderer->cleanScreen();
    mTAAController->setView(mCamera->getViewMatrix());
    mTAAController->makeJitteredProjection();
    writeSceneToShadowMap();
    writeSceneToTAATextures();
    const auto& screenTexture = resolveTAA();
    renderFinalScreenTexture(screenTexture);
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
    const auto& bbox = mRootObject.getBBox();
    const auto& bboxCenter = bbox.getCenter();
    const auto& bboxMax = bbox.getMax();
    auto yOffset = bboxMax.y * 0.2f;
    return {bboxCenter.x, bboxMax.y + yOffset, bboxCenter.z};
  }

  const Object3D& Scene::getRootObject() const { return mRootObject; }

  std::vector<ViewportListener*> Scene::getViewportListeners()
  {
    return {this, mShadowController.get(), mTAAController.get(), mSkyboxController.get()};
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
      viewport->getProjectionType(), mRootObject.getBBox(), viewport->getFov()
    );
  }
}  // namespace RenderSystem
