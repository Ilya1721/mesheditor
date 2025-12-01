#pragma once

#include <functional>
#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"
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
    const Point3D& getEye() const;
    Point3D projectToTargetPlane(const Point3D& cursorPosInWorldSpace) const;
    bool isMovementEnabled() const;

    void pan(
      const Point3D& startPointInWorldSpace,
      const Point3D& endPointInWorldSpace,
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
    void setEyeTargetUp(const Point3D& eye, const Point3D& target, const Vector3D& up);
    void translate(const Vector3D& movement);
    void adjust(const MeshCore::AABBox& bbox, float fov = 0.0f);
    void rotateCamera(const glm::mat4& rotationTransform);
    void rawOrbit(const Point3D& startPointInNDC, const Point3D& endPointInNDC);
    void perspectiveAdjust(const MeshCore::AABBox& bbox, float fov);
    void orthoAdjust(const MeshCore::AABBox& bbox);
    void validateCamera() const;

    Vector3D getNormalizedDirection() const;
    Vector3D calcRight() const;
    glm::mat4 createViewMatrix() const;
    glm::mat4 calculateViewMatrixWithTargetAtOrigin() const;
    Plane getTargetPlane() const;
    Point3D getCursorPosInNDCWithZ(const Point3D& cursorPosInNDC) const;
    glm::mat4 getOrbitTransform(
      const Point3D& startPosInNDCWithZ, const Point3D& endPosInNDCWithZ
    ) const;
    glm::mat4 getSmoothOrbitTransform(float yaw, float pitch) const;
    glm::mat4 getCorrectionOrbitTransform() const;
    float calculateDistanceToCamera(const MeshCore::AABBox& bbox, float fov = 0.0f) const;
    glm::vec3 getPanTranslationVector(
      const Point3D& startPointInWorldSpace,
      const Point3D& endPointInWorldSpace,
      PROJECTION_TYPE projectionType
    ) const;

   private:
    Point3D mTarget;
    Point3D mEye;
    Vector3D mUp;
    Vector3D mRight;
    glm::mat4 mViewMatrix;
    bool mIsMovementEnabled;
    CallbackMechanism<cameraPosChangedCallback> mCameraPosChangedCallbacks;
  };
}  // namespace RenderSystem
