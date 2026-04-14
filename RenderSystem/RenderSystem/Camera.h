#pragma once

#include <functional>
#include <glm/glm.hpp>

#include "Utility/CallbackMechanism.h"
#include "Viewport.h"

namespace GeometryCore
{
  struct Ray;
  struct Plane;
}  // namespace GeometryCore

namespace MeshCore
{
  class AABBox;
}

namespace RenderSystem
{
  using namespace GeometryCore;
  using namespace Utility;

  using cameraPosChangedCallback = void();

  class Camera
  {
   public:
    Camera();

    const glm::mat4& getViewMatrix() const;
    const glm::vec3& getEye() const;
    glm::vec3 projectToTargetPlane(const glm::vec3& cursorPosInWorldSpace) const;
    bool isMovementEnabled() const;

    void pan(
      const glm::vec3& startPointInWorldSpace,
      const glm::vec3& endPointInWorldSpace,
      PROJECTION_TYPE projectionType
    );
    void smoothOrbit(float xOffset, float yOffset);
    void zoom(float step);
    void enableMovement(bool isEnabled);
    void adjust(
      PROJECTION_TYPE projectionType, const MeshCore::AABBox& sceneBBox, float fov
    );
    void addOnCameraPosChangedCallback(
      const std::function<cameraPosChangedCallback>& callback
    );

   private:
    void invokeEditOperation(const std::function<void()>& action);
    void setEyeTargetUp(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);
    void translate(const glm::vec3& movement);
    void adjust(const MeshCore::AABBox& bbox, float fov = 0.0f);
    void rotateCamera(const glm::mat4& rotationTransform);
    void rawOrbit(const glm::vec3& startPointInNDC, const glm::vec3& endPointInNDC);
    void perspectiveAdjust(const MeshCore::AABBox& bbox, float fov);
    void orthoAdjust(const MeshCore::AABBox& bbox);
    void validateCamera() const;

    glm::vec3 getNormalizedDirection() const;
    glm::vec3 calcRight() const;
    glm::mat4 createViewMatrix() const;
    glm::mat4 calculateViewMatrixWithTargetAtOrigin() const;
    Plane getTargetPlane() const;
    glm::vec3 getCursorPosInNDCWithZ(const glm::vec3& cursorPosInNDC) const;
    glm::mat4 getOrbitTransform(
      const glm::vec3& startPosInNDCWithZ, const glm::vec3& endPosInNDCWithZ
    ) const;
    glm::mat4 getSmoothOrbitTransform(float yaw, float pitch) const;
    glm::mat4 getCorrectionOrbitTransform() const;
    float calculateDistanceToCamera(const MeshCore::AABBox& bbox, float fov = 0.0f) const;
    glm::vec3 getPanTranslationVector(
      const glm::vec3& startPointInWorldSpace,
      const glm::vec3& endPointInWorldSpace,
      PROJECTION_TYPE projectionType
    ) const;

   private:
    glm::vec3 mTarget;
    glm::vec3 mEye;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::mat4 mViewMatrix;
    bool mIsMovementEnabled;
    CallbackMechanism<cameraPosChangedCallback> mCameraPosChangedCallbacks;
  };
}  // namespace RenderSystem
