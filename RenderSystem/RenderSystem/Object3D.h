#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "MeshCore/AABBox.h"
#include "MeshCore/Mesh.h"
#include "MeshCore/TreeWalker.h"
#include "MeshCore/Vertex.h"
#include "Object3DIntersection.h"
#include "RenderData.h"
#include "Utility/CallbackMechanism.h"

using namespace MeshCore;

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
    Object3D(std::unique_ptr<Mesh> mesh);
    Object3D(Object3D&& other) = delete;
    ~Object3D();

    Object3D* getParent() const;
    const std::vector<std::unique_ptr<Object3D>>& getChildren() const;
    const glm::mat4& getTransform() const;
    const AABBox& getBBox() const;
    Object3DIntersection getIntersection(
      const GeometryCore::Ray& ray,
      IntersectionMode intersectionMode,
      int facesIndexOffset = 0
    );
    std::unique_ptr<Object3D> clone();
    int getVertexCount() const;
    const std::vector<Vertex>& getVertices() const;

    void addChild(std::unique_ptr<Object3D>&& child);
    void addOnChildAddedCallback(const std::function<childAddedCallback>& callback);
    void addOnObjectUpdatedCallback(const std::function<objectUpdatedCallback>& callback);
    void addOnBBoxUpdatedCallback(const std::function<bboxUpdatedCallback>& callback);
    void updateTransform(const glm::mat4& transform);
    void onMeshUpdated(const std::unordered_set<UniqueVertex*>& vertices);
    void moveToOrigin();

   protected:
    void init();
    void invokeTransformAction(
      const std::function<void()>& action, const glm::mat4& transform
    );
    void propagateBBoxToRoot();

   protected:
    Object3D* mParent;
    std::vector<std::unique_ptr<Object3D>> mChildren;
    std::unique_ptr<Mesh> mMesh;
    glm::mat4 mTransform;
    AABBox mBBox;
    Utility::CallbackMechanism<childAddedCallback> mChildAddedCM;
    Utility::CallbackMechanism<objectUpdatedCallback> mObjectUpdatedCM;
    Utility::CallbackMechanism<bboxUpdatedCallback> mBBoxUpdatedCM;
  };
}  // namespace RenderSystem
