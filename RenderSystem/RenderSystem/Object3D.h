#pragma once

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include <functional>
#include <memory>
#include <vector>

#include "Animation.h"
#include "Material.h"
#include "MeshCore/AABBox.h"
#include "MeshCore/Mesh.h"
#include "MeshCore/Vertex.h"
#include "Object3DIntersection.h"
#include "Utility/CallbackMechanism.h"

using namespace MeshCore;
using namespace Utility;

namespace RenderSystem
{
  using childAddedCallback = void(const Object3D*);
  using objectUpdatedCallback =
    void(const Object3D*, const std::unordered_set<UniqueVertex*>&);
  using bboxUpdatedCallback = void();

  class Object3D
  {
   public:
    Object3D();
    Object3D(
      std::unique_ptr<Mesh> mesh,
      const Material& material,
      const glm::mat4& transform = glm::mat4(1.0f),
      int renderMode = GL_TRIANGLES
    );
    Object3D(
      std::unique_ptr<Mesh> mesh,
      const Material& material,
      Skeleton&& skeleton,
      std::vector<Animation>&& animations,
      const glm::mat4& transform = glm::mat4(1.0f),
      int renderMode = GL_TRIANGLES
    );
    Object3D(Object3D&& other) = delete;
    virtual ~Object3D();

    size_t getVertexCount() const;
    const glm::mat4& getTransform() const;

    Object3D* getParent() const;
    const std::vector<std::unique_ptr<Object3D>>& getChildren() const;
    const AABBox& getBBox() const;
    Object3DIntersection getRayIntersection(
      const GeometryCore::Ray& ray,
      IntersectionMode intersectionMode,
      int facesIndexOffset = 0
    );
    std::unique_ptr<Object3D> clone();
    const std::vector<Vertex>& getVertices() const;
    const Material& getMaterial() const;
    const Skeleton& getSkeleton() const;
    const std::vector<Animation>& getAnimations() const;
    int getRenderMode() const;

    void addChild(std::unique_ptr<Object3D>&& child);
    void addOnChildAddedCallback(const std::function<childAddedCallback>& callback);
    void addOnObjectUpdatedCallback(const std::function<objectUpdatedCallback>& callback);
    void addOnBBoxUpdatedCallback(const std::function<bboxUpdatedCallback>& callback);
    void onMeshUpdated(const std::unordered_set<UniqueVertex*>& vertices);
    void moveToOrigin();
    void setMaterial(const Material& material);
    void setUVScale(const glm::vec2& uvScale);
    void setRenderMode(int renderMode);
    void updateTransform(const glm::mat4& transform);

   protected:
    void init();
    void propagateBBoxToRoot();

   protected:
    Object3D* mParent;
    std::vector<std::unique_ptr<Object3D>> mChildren;
    std::unique_ptr<Mesh> mMesh;
    glm::mat4 mTransform;
    AABBox mBBox;
    std::shared_ptr<Material> mMaterial;
    Skeleton mSkeleton;
    std::vector<Animation> mAnimations;
    int mRenderMode;
    CallbackMechanism<childAddedCallback> mChildAddedCM;
    CallbackMechanism<objectUpdatedCallback> mObjectUpdatedCM;
    CallbackMechanism<bboxUpdatedCallback> mBBoxUpdatedCM;
  };
}  // namespace RenderSystem
