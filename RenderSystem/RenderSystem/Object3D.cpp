#include "Object3D.h"

#include <glm/gtx/transform.hpp>

#include "GeometryCore/Ray.h"
#include "MeshCore/Mesh.h"
#include "MeshCore/TreeWalker.h"

using namespace GeometryCore;

namespace RenderSystem
{
  Object3D::Object3D()
    : mParent(nullptr),
      mTransform(1.0f),
      mRenderMode(GL_TRIANGLES),
      mMaterial(std::make_shared<Material>())
  {
  }

  Object3D::Object3D(
    std::unique_ptr<Mesh> mesh,
    const Material& material,
    const glm::mat4& transform,
    int renderMode
  )
    : mParent(nullptr),
      mMesh(std::move(mesh)),
      mMaterial(material.clone()),
      mTransform(transform),
      mRenderMode(renderMode)
  {
    init();
  }

  Object3D::Object3D(
    std::unique_ptr<Mesh> mesh,
    const Material& material,
    Skeleton&& skeleton,
    std::vector<Animation>&& animations,
    const glm::mat4& transform,
    int renderMode
  )
    : mParent(nullptr),
      mMesh(std::move(mesh)),
      mMaterial(material.clone()),
      mTransform(transform),
      mSkeleton(std::move(skeleton)),
      mAnimations(std::move(animations)),
      mRenderMode(renderMode)
  {
    init();
  }

  Object3D::~Object3D() = default;

  void Object3D::init()
  {
    mBBox.applyMesh(*mMesh);
    moveToOrigin();
  }

  Object3D* Object3D::getParent() const
  {
    return mParent;
  }

  const std::vector<std::unique_ptr<Object3D>>& Object3D::getChildren() const
  {
    return mChildren;
  }

  const glm::mat4& Object3D::getTransform() const
  {
    return mTransform;
  }

  void Object3D::updateTransform(const glm::mat4& transform)
  {
    mTransform = transform * mTransform;
    mBBox.applyTransform(transform);
    propagateBBoxToRoot();
  }

  const AABBox& Object3D::getBBox() const
  {
    return mBBox;
  }

  Object3DIntersection Object3D::getRayIntersection(
    const Ray& ray, IntersectionMode intersectionMode, int facesIndexOffset
  )
  {
    Object3DIntersection closestIntersection;
    TreeWalker walker(this);
    walker.forEach(
      [&ray, &closestIntersection, &intersectionMode, &facesIndexOffset,
       this](Object3D* object)
      {
        const auto& faces = object->mMesh->getFaces();
        const auto currentFacesIndexOffset = faces.size() + facesIndexOffset;
        auto rayLocalSpace = glm::inverse(object->getTransform()) * ray;
        auto meshIntersection = object->mMesh->getRayIntersection(
          rayLocalSpace, intersectionMode, currentFacesIndexOffset
        );
        facesIndexOffset += faces.size();

        if (meshIntersection.intersectedFacesIndices.empty())
        {
          return;
        }

        meshIntersection.intersectionPoint =
          object->getTransform() * glm::vec4(meshIntersection.intersectionPoint, 1.0f);

        if (!closestIntersection.intersectedObject ||
            isCloser(
              meshIntersection.intersectionPoint,
              closestIntersection.raySurfaceIntersection.point, ray.origin
            ))
        {
          closestIntersection.intersectedObject = object;
          closestIntersection.raySurfaceIntersection.assign(meshIntersection);
        }
      }
    );

    return closestIntersection;
  }

  std::unique_ptr<Object3D> Object3D::clone()
  {
    auto newObject = std::make_unique<Object3D>();
    newObject->mMesh = mMesh->clone();
    newObject->mBBox = mBBox;
    newObject->mTransform = mTransform;
    newObject->mParent = mParent;
    newObject->mMaterial = mMaterial->clone();
    newObject->mSkeleton = mSkeleton;
    newObject->mAnimations = mAnimations;

    for (const auto& child : mChildren)
    {
      newObject->addChild(child->clone());
    }

    return newObject;
  }

  size_t Object3D::getVertexCount() const
  {
    return mMesh->getVertices().size();
  }

  const std::vector<Vertex>& Object3D::getVertices() const
  {
    return mMesh->getVertices();
  }

  const Material& Object3D::getMaterial() const
  {
    return *mMaterial;
  }

  const Skeleton& Object3D::getSkeleton() const
  {
    return mSkeleton;
  }

  const std::vector<Animation>& Object3D::getAnimations() const
  {
    return mAnimations;
  }

  int Object3D::getRenderMode() const
  {
    return mRenderMode;
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

  void Object3D::moveToOrigin()
  {
    updateTransform(glm::translate(-mBBox.getCenter()));
  }

  void Object3D::setMaterial(const Material& material)
  {
    mMaterial = std::make_shared<Material>(material);
  }

  void Object3D::setUVScale(const glm::vec2& uvScale)
  {
    mMaterial->setUVScale(uvScale);
  }

  void Object3D::setRenderMode(int renderMode)
  {
    mRenderMode = renderMode;
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
