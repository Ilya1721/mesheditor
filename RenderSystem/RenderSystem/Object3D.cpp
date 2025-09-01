#include "Object3D.h"

#include <MeshCore/TreeWalker.h>

#include <glm/gtx/transform.hpp>

#include "GeometryCore/Ray.h"
#include "MeshCore/Mesh.h"

using namespace GeometryCore;

namespace RenderSystem
{
  Object3D::Object3D() : mParent(nullptr), mTransform(1.0f) {}

  Object3D::Object3D(std::unique_ptr<Mesh> mesh)
    : mParent(nullptr),
      mMesh(std::move(mesh)),
      mTransform(1.0f),
      mMaterialParams(mMesh->getMaterialParams())
  {
    init();
  }

  Object3D::~Object3D() = default;

  void Object3D::init()
  {
    mBBox.applyMesh(*mMesh);
    moveToOrigin();
  }

  Object3D* Object3D::getParent() const { return mParent; }

  const std::vector<std::unique_ptr<Object3D>>& Object3D::getChildren() const
  {
    return mChildren;
  }

  const glm::mat4& Object3D::getTransform() const { return mTransform; }

  void Object3D::updateTransform(const glm::mat4& transform)
  {
    invokeTransformAction(
      [this, &transform]() { mTransform = transform * mTransform; }, transform
    );
  }

  const AABBox& Object3D::getBBox() const { return mBBox; }

  Object3DIntersection Object3D::getIntersection(
    const Ray& ray, IntersectionMode intersectionMode, int facesIndexOffset
  )
  {
    Object3DIntersection objectIntersection;
    TreeWalker walker(this);
    walker.forEach(
      [&ray, &objectIntersection, &intersectionMode, &facesIndexOffset,
       this](Object3D* object)
      {
        const auto& faces = object->mMesh->getFaces();
        const auto currentFacesIndexOffset = faces.size() + facesIndexOffset;
        auto invertedRay = glm::inverse(object->getTransform()) * ray;
        auto meshIntersection = object->mMesh->getIntersection(
          invertedRay, intersectionMode, currentFacesIndexOffset
        );
        facesIndexOffset += faces.size();

        if (meshIntersection.intersectedFacesIndices.empty()) { return; }

        if (isCloser(
              meshIntersection.intersectionPoint,
              objectIntersection.raySurfaceIntersection.point, invertedRay.origin
            ))
        {
          objectIntersection.intersectedObject = object;
          objectIntersection.raySurfaceIntersection.assign(meshIntersection);
        }
      }
    );

    return objectIntersection;
  }

  std::unique_ptr<Object3D> Object3D::clone()
  {
    auto newObject = std::make_unique<Object3D>();
    newObject->mMesh = mMesh->clone();
    newObject->mBBox = mBBox;
    newObject->mTransform = mTransform;
    newObject->mParent = mParent;

    for (const auto& child : mChildren) { newObject->addChild(child->clone()); }

    return newObject;
  }

  int Object3D::getVertexCount() const { return mMesh->getVertices().size(); }

  const std::vector<Vertex>& Object3D::getVertices() const
  {
    return mMesh->getVertices();
  }

  const Object3DMaterialParams& Object3D::getMaterialParams() const
  {
    return mMaterialParams;
  }

  void Object3D::addChild(std::unique_ptr<Object3D>&& child)
  {
    child->mParent = this;
    child->propagateBBoxToRoot();
    child->mChildAddedCM.addCallbacks(mChildAddedCM);
    child->mObjectUpdatedCM.addCallbacks(mObjectUpdatedCM);
    mChildAddedCM.invokeCallbacks(child.get());
    mChildren.emplace_back(std::move(child));
  }

  void Object3D::addOnChildAddedCallback(const std::function<childAddedCallback>& callback
  )
  {
    mChildAddedCM.addCallback(callback);
  }

  void Object3D::addOnObjectUpdatedCallback(
    const std::function<objectUpdatedCallback>& callback
  )
  {
    mObjectUpdatedCM.addCallback(callback);
  }

  void Object3D::addOnBBoxUpdatedCallback(
    const std::function<bboxUpdatedCallback>& callback
  )
  {
    mBBoxUpdatedCM.addCallback(callback);
  }

  void Object3D::onMeshUpdated(const std::unordered_set<UniqueVertex*>& vertices)
  {
    mBBox.applyMesh(*mMesh);
    propagateBBoxToRoot();
    mObjectUpdatedCM.invokeCallbacks(this, vertices);
  }

  void Object3D::moveToOrigin() { updateTransform(glm::translate(-mBBox.getCenter())); }

  void Object3D::invokeTransformAction(
    const std::function<void()>& action, const glm::mat4& transform
  )
  {
    action();
    mBBox.applyTransform(transform);
    propagateBBoxToRoot();
  }

  void Object3D::propagateBBoxToRoot()
  {
    auto currentObject = this;
    while (currentObject->mParent != nullptr)
    {
      currentObject->mParent->mBBox.applyBBox(currentObject->mBBox);
      currentObject = currentObject->mParent;
    }
    currentObject->mBBoxUpdatedCM.invokeCallbacks();
  }
}  // namespace RenderSystem
