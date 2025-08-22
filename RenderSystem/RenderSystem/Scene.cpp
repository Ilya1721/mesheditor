#include "Scene.h"

#include <glm/gtx/transform.hpp>

#include "Constants.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Transforms.h"
#include "MeshCore/Intersection.h"
#include "MeshFilesLoader/MeshFilesLoader.h"
#include "PointLightObject3D.h"
#include "Renderer.h"
#include "SceneShaderProgram.h"
#include "ShadowController.h"

using namespace GeometryCore;

namespace RenderSystem
{
  constexpr MaterialParams FLOOR_MATERIAL = PEARL_MATERIAL;
  constexpr MaterialParams MAIN_MATERIAL = GOLD_MATERIAL;
  constexpr MaterialParams HIGHLIGHT_MATERIAL = RUBY_MATERIAL;
  constexpr MaterialParams WIREFRAME_MATERIAL = BLACK_MATERIAL;
}  // namespace RenderSystem

namespace RenderSystem
{
  Scene::Scene(
    const std::string& meshFilePath,
    Renderer* renderer,
    ShadowController* shadowController,
    SceneShaderProgram* sceneShaderProgram,
    float aspectRatio
  )
    : mRenderer(renderer),
      mShadowController(shadowController),
      mSceneShaderProgram(sceneShaderProgram),
      mPickedObject(nullptr),
      mRenderWireframe(false),
      mHighlightedObject(nullptr),
      mDirLightSource(sceneShaderProgram, shadowController),
      mAspectRatio(aspectRatio)
  {
    registerRootObjectCallbacks();
    addModelObject(meshFilePath);
  }

  void Scene::addModelObject(const std::string& meshFilePath)
  {
    auto modelObject = std::make_unique<Object3D>(MeshFilesLoader::loadSTL(meshFilePath));
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
      shaderProgram->setModel(object->getTransform());
      mRenderer->renderObject3D(*object, vertexOffset);
    }
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

    int startIndex = 0;
    for (const auto& sceneDecoration : mSceneDecorations)
    {
      mSceneShaderProgram->setMaterialParams(sceneDecoration.materialParams);
      mRenderer->renderSceneDecoration(sceneDecoration, startIndex);
    }

    mSceneShaderProgram->setMaterialParams(MAIN_MATERIAL);
  }

  void Scene::renderHighlightedFaces()
  {
    mSceneShaderProgram->setMaterialParams(HIGHLIGHT_MATERIAL);

    for (const auto& faceIdx : mHighlightedFacesData.facesIndices)
    {
      mSceneShaderProgram->setModel(mHighlightedFacesData.parentObject->getTransform());
      mRenderer->renderHighlightedFace(
        faceIdx, mSceneObjectVertexOffsetMap.at(mHighlightedFacesData.parentObject)
      );
    }

    mSceneShaderProgram->setMaterialParams(MAIN_MATERIAL);
  }

  void Scene::renderWireframe()
  {
    if (!mRenderWireframe) { return; }

    mSceneShaderProgram->setMaterialParams(WIREFRAME_MATERIAL);

    for (const auto& [object, vertexOffset] : mSceneObjectVertexOffsetMap)
    {
      mSceneShaderProgram->setModel(object->getTransform());
      mRenderer->renderWireframe(object->getVertexCount());
    }

    mSceneShaderProgram->setMaterialParams(MAIN_MATERIAL);
  }

  void Scene::renderWholeObjectHighlighted()
  {
    const auto& highlightedObjectIt =
      mSceneObjectVertexOffsetMap.find(mHighlightedObject);
    if (highlightedObjectIt == mSceneObjectVertexOffsetMap.end()) { return; }

    mSceneShaderProgram->setMaterialParams(HIGHLIGHT_MATERIAL);
    const auto& [object, vertexCount] = *highlightedObjectIt;
    mSceneShaderProgram->setModel(object->getTransform());
    mRenderer->renderWholeObjectHighlighted(*object, vertexCount);
    mSceneShaderProgram->setMaterialParams(MAIN_MATERIAL);
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

  void Scene::setPickedObject(Object3D* pickedObject) { mPickedObject = pickedObject; }

  void Scene::addSceneDecoration(const SceneDecoration& sceneDecoration)
  {
    mSceneDecorations.push_back(sceneDecoration);
    mSceneDecorationsRenderData.append(sceneDecoration.renderData);
    mRenderer->loadDecorationsRenderData(mSceneDecorationsRenderData);
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
    auto pointLightObject3D = std::make_unique<PointLightObject3D>(pointLight);
    pointLightObject3D->updateTransform(glm::translate(lightSourcePos));
    mRootObject.addChild(std::move(pointLightObject3D));
  }

  void Scene::removePointLight(unsigned int index)
  {
    mSceneShaderProgram->removePointLight(index);
  }

  void Scene::setPointLightParams(unsigned int index, const PointLightParams& params)
  {
    mSceneShaderProgram->setPointLightParams(index, params);
  }

  void Scene::setPointLightSourcePos(unsigned int index, const Point3D& lightSourcePos)
  {
    mSceneShaderProgram->setPointLightSourcePos(index, lightSourcePos);
  }

  void Scene::render()
  {
    renderDepthMap();
    mSceneShaderProgram->setDepthMap(mShadowController->getDepthTextureId());
    mSceneShaderProgram->invoke(
      [this]()
      {
        mRenderer->cleanScreen();
        renderScene(mSceneShaderProgram);
        renderSceneDecorations();
        renderHighlightedFaces();
        renderWireframe();
        renderWholeObjectHighlighted();
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
