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

using namespace GeometryCore;

namespace RenderSystem
{
  const BlinnPhongMaterialParams FLOOR_MATERIAL = PEARL_MATERIAL;
  const BlinnPhongMaterialParams HIGHLIGHT_MATERIAL = RUBY_MATERIAL;
  const BlinnPhongMaterialParams WIREFRAME_MATERIAL = BLACK_MATERIAL;
}  // namespace RenderSystem

namespace RenderSystem
{
  Scene::Scene(
    const std::string& meshFilePath,
    Renderer* renderer,
    ShadowController* shadowController,
    SkyboxController* skyboxController,
    SceneShaderProgram* sceneShaderProgram,
    float aspectRatio
  )
    : mRenderer(renderer),
      mShadowController(shadowController),
      mSkyboxController(skyboxController),
      mSceneShaderProgram(sceneShaderProgram),
      mPickedObject(nullptr),
      mRenderWireframe(false),
      mHighlightedObject(nullptr),
      mDirLightSource(sceneShaderProgram, shadowController),
      mAspectRatio(aspectRatio)
  {
    registerRootObjectCallbacks();
    addModelObject(meshFilePath);
    loadSkyboxVertices();
  }

  void Scene::addModelObject(const std::string& meshFilePath)
  {
    auto modelObject =
      std::make_unique<Object3D>(MeshFilesLoader::loadMeshFromFile(meshFilePath));
    mRootObject.addChild(std::move(modelObject));
    addSceneDecoration(
      SceneDecoration::createSceneFloor(mRootObject.getBBox().getHeight(), FLOOR_MATERIAL)
    );
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

  void Scene::renderScene(AbstractShaderProgram* shaderProgram)
  {
    for (auto& [object, vertexOffset] : mSceneObjectVertexOffsetMap)
    {
      const auto& materialParams = object->getMaterialParams();
      shaderProgram->setModel(object->getTransform());
      mSceneShaderProgram->setBlinnPhongMaterialParams(
        materialParams.blinnPhongMaterialParams
      );
      mSceneShaderProgram->setGlassMaterialParams(materialParams.glassMaterialParams);
      mSceneShaderProgram->setMaterialType(materialParams.materialType);
      mRenderer->renderObject3D(*object, vertexOffset);
    }
    mSceneShaderProgram->setMaterialType(MaterialType::BLINN_PHONG);
  }

  void Scene::renderDepthMap()
  {
    mShadowController->renderSceneToDepthMap(
      [this]()
      {
        renderScene(mShadowController->getShaderProgram());
        renderSceneDecorations();
      }
    );
  }

  void Scene::renderSceneDecorations()
  {
    mSceneShaderProgram->setModel(glm::mat4(1.0f));
    mShadowController->getShaderProgram()->setModel(glm::mat4(1.0f));

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
    mShadowController->getShaderProgram()->setLightProjection(lightProjectionMatrix);
  }

  void Scene::loadSkyboxVertices()
  {
    mRenderer->loadSkyboxRenderData(RenderData::getSkyboxRenderData());
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

  void Scene::setDirLightSourcePos(const Point3D& pos)
  {
    mDirLightSource.setPosition(pos);
  }

  void Scene::setAspectRatio(float aspectRatio) { mAspectRatio = aspectRatio; }

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
    renderDepthMap();
    renderSkybox();

    mSceneShaderProgram->setDepthMap(mShadowController->getDepthMap());
    mSceneShaderProgram->invoke(
      [this]()
      {
        renderSceneDecorations();
        renderHighlightedFaces();
        renderWireframe();
        renderWholeObjectHighlighted();
        renderScene(mSceneShaderProgram);
      }
    );
  }

  Object3DIntersection Scene::getIntersection(
    const Ray& cursorRay, IntersectionMode intersectionMode
  )
  {
    if (mRootObject.getBBox().getIntersectionPoint(cursorRay))
    {
      return mRootObject.getIntersection(cursorRay, intersectionMode);
    }

    return {};
  }

  Object3D* Scene::getPickedObject() const { return mPickedObject; }

  const std::vector<SceneDecoration>& Scene::getSceneDecorations() const
  {
    return mSceneDecorations;
  }

  bool Scene::getRenderWireframe() const { return mRenderWireframe; }

  const Object3D* Scene::getHighlightedObject() { return mHighlightedObject; }

  const HighlightedFacesData& Scene::getHighlightedFacesData()
  {
    return mHighlightedFacesData;
  }

  Point3D Scene::getDefaultPointLightSourcePos() const
  {
    const auto& bbox = mRootObject.getBBox();
    const auto& bboxCenter = bbox.getCenter();
    const auto& bboxMax = bbox.getMax();
    auto yOffset = bboxMax.y * 0.2f;
    return {bboxCenter.x, bboxMax.y + yOffset, bboxCenter.z};
  }

  const Object3D& Scene::getRootObject() const { return mRootObject; }
}  // namespace RenderSystem
