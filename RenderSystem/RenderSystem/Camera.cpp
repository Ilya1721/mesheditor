#include "Camera.h"

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Constants.h"
#include "GeometryCore/Constants.h"
#include "GeometryCore/Numeric.h"
#include "GeometryCore/Plane.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Transforms.h"
#include "MeshCore/AABBox.h"

using namespace GeometryCore;

namespace RenderSystem
{
  Camera::Camera()
    : mTarget(DEFAULT_CAMERA_TARGET),
      mEye(DEFAULT_CAMERA_POSITION),
      mUp(DEFAULT_CAMERA_UP),
      mRight(DEFAULT_CAMERA_RIGHT),
      mIsMovementEnabled(true)
  {
    invokeEditOperation([]() {});
  }

  const glm::mat4& Camera::getViewMatrix() const { return mViewMatrix; }

  glm::mat4 Camera::createViewMatrix() const { return glm::lookAt(mEye, mTarget, mUp); }

  const Point3D& Camera::getEye() const { return mEye; }

  Vector3D Camera::getNormalizedDirection() const
  {
    return glm::normalize(mTarget - mEye);
  }

  Point3D Camera::projectToTargetPlane(const Point3D& cursorPosInWorldSpace) const
  {
    Ray cursorRay(cursorPosInWorldSpace, cursorPosInWorldSpace - mEye);
    return getTargetPlane().getIntersectionPoint(cursorRay).value();
  }

  bool Camera::isMovementEnabled() const { return mIsMovementEnabled; }

  void Camera::invokeEditOperation(const std::function<void()>& action)
  {
    action();
    mViewMatrix = createViewMatrix();
    mCameraPosChangedCallbacks.invokeCallbacks();
  }

  void Camera::setEyeTargetUp(
    const Point3D& eye, const Point3D& target, const Vector3D& up
  )
  {
    mEye = eye;
    mTarget = target;
    mUp = glm::normalize(up);
    validateCamera();
    mRight = calcRight();
  }

  void Camera::pan(
    const Point3D& startPointInWorldSpace,
    const Point3D& endPointInWorldSpace,
    PROJECTION_TYPE projectionType
  )
  {
    invokeEditOperation(
      [this, &startPointInWorldSpace, &endPointInWorldSpace, &projectionType]()
      {
        translate(getPanTranslationVector(
          startPointInWorldSpace, endPointInWorldSpace, projectionType
        ));
      }
    );
  }

  void Camera::zoom(float step)
  {
    invokeEditOperation([&step, this]() { mEye += glm::normalize(mTarget - mEye) * step; }
    );
  }

  void Camera::translate(const Vector3D& movement)
  {
    mEye += movement;
    mTarget += movement;
  }

  glm::mat4 Camera::calculateViewMatrixWithTargetAtOrigin() const
  {
    return glm::lookAt(mEye - mTarget, Point3D(0.0, 0.0, 0.0), mUp);
  }

  Plane Camera::getTargetPlane() const { return {mTarget, mEye - mTarget}; }

  Point3D Camera::getCursorPosInNDCWithZ(const Point3D& cursorPosInNDC) const
  {
    Point3D cursorPosInNDCWithZ = cursorPosInNDC;
    auto lengthSquared = std::powf(cursorPosInNDC.x, 2) + std::powf(cursorPosInNDC.y, 2);
    auto validZ =
      std::abs(sqrtf(1 - std::powf(cursorPosInNDC.x, 2) - std::powf(cursorPosInNDC.y, 2))
      );
    cursorPosInNDCWithZ.z = lengthSquared > 1.0f ? 0.0f : validZ;

    return cursorPosInNDCWithZ;
  }

  glm::mat4 Camera::getOrbitTransform(
    const Point3D& startPosInNDCWithZ, const Point3D& endPosInNDCWithZ
  ) const
  {
    auto rotationAngle =
      glm::angle(glm::normalize(startPosInNDCWithZ), glm::normalize(endPosInNDCWithZ));
    if (glm::epsilonEqual(rotationAngle, 0.0f, 1e-5f)) { return glm::mat4(1.0f); }

    auto rotationAxisInNDC = glm::cross(startPosInNDCWithZ, endPosInNDCWithZ);
    auto rotationAxisInCameraSpace = transformVector(
      rotationAxisInNDC, glm::inverse(calculateViewMatrixWithTargetAtOrigin())
    );
    auto unitRotationAxisInCameraSpace = glm::normalize(rotationAxisInCameraSpace);

    return glm::rotate(-rotationAngle * ORBIT_SPEED_COEF, unitRotationAxisInCameraSpace);
  }

  glm::mat4 Camera::getSmoothOrbitTransform(float yaw, float pitch) const
  {
    return glm::rotate(pitch, mRight) * glm::rotate(-yaw, Vector3D(0.0f, 1.0f, 0.0f));
  }

  glm::mat4 Camera::getCorrectionOrbitTransform() const
  {
    Vector3D yAxis(0.0f, 1.0f, 0.0f);
    auto angle = glm::angle(mRight, yAxis) - HALF_PI;
    auto rotationAxis = glm::cross(mRight, yAxis);

    return glm::rotate(angle, rotationAxis);
  }

  void Camera::validateCamera() const
  {
    Vector3D nullVector(0.0f, 0.0f, 0.0f);

    if (isEqual(mUp, nullVector) || isEqual(mRight, nullVector))
    {
      throw std::exception("Either up or right camera vector is null vector");
    }

    if (glm::any(glm::isnan(mEye)) || glm::any(glm::isnan(mTarget)) ||
        glm::any(glm::isnan(mUp)) || glm::any(glm::isnan(mRight)))
    {
      throw std::exception(
        "Some important Camera vector contains coordinates that are not numbers"
      );
    }
  }

  float Camera::calculateDistanceToCamera(const MeshCore::AABBox& bbox, float fov) const
  {
    return bbox.getHeight() / (2.0f * glm::tan(glm::radians(fov / 2.0f))) *
           CAMERA_DIST_TO_BBOX_COEF;
  }

  glm::vec3 Camera::getPanTranslationVector(
    const Point3D& startPointInWorldSpace,
    const Point3D& endPointInWorldSpace,
    PROJECTION_TYPE projectionType
  ) const
  {
    if (projectionType == PROJECTION_TYPE::PERSPECTIVE)
    {
      return projectToTargetPlane(startPointInWorldSpace) -
             projectToTargetPlane(endPointInWorldSpace);
    }

    return startPointInWorldSpace - endPointInWorldSpace;
  }

  Vector3D Camera::calcRight() const
  {
    auto directionNormalized = getNormalizedDirection();
    if (glm::epsilonNotEqual(glm::dot(directionNormalized, mUp), 0.0f, 1e-5f))
    {
      throw std::exception(
        "Camera Up and Direction vectors must be perpendicular to each other"
      );
    }

    return glm::cross(directionNormalized, mUp);
  }

  void Camera::adjust(const MeshCore::AABBox& bbox, float fov)
  {
    invokeEditOperation(
      [&bbox, &fov, this]()
      {
        auto distanceToCamera = fov > 0.0f ? calculateDistanceToCamera(bbox, fov) : 0.0f;
        Point3D eye(mEye.x, mEye.y, bbox.getMax().z + distanceToCamera);
        setEyeTargetUp(eye, mTarget, mUp);
      }
    );
  }

  void Camera::rotateCamera(const glm::mat4& rotationTransform)
  {
    auto cameraInverseDir = transformVector(mEye - mTarget, rotationTransform);
    auto up = transformVector(mUp, rotationTransform);
    setEyeTargetUp(mTarget + cameraInverseDir, mTarget, up);
  }

  void Camera::rawOrbit(const Point3D& startPointInNDC, const Point3D& endPointInNDC)
  {
    invokeEditOperation(
      [&startPointInNDC, &endPointInNDC, this]()
      {
        auto startPointInNDCWithZ = getCursorPosInNDCWithZ(startPointInNDC);
        auto endPointInNDCWithZ = getCursorPosInNDCWithZ(endPointInNDC);
        rotateCamera(getOrbitTransform(startPointInNDCWithZ, endPointInNDCWithZ));
      }
    );
  }

  void Camera::smoothOrbit(float xOffset, float yOffset)
  {
    invokeEditOperation(
      [&xOffset, &yOffset, this]()
      {
        rotateCamera(getSmoothOrbitTransform(
          xOffset * ARCBALL_SENSITIVITY, yOffset * ARCBALL_SENSITIVITY
        ));
        rotateCamera(getCorrectionOrbitTransform());
      }
    );
  }

  void Camera::perspectiveAdjust(const MeshCore::AABBox& bbox, float fov)
  {
    adjust(bbox, fov);
  }

  void Camera::orthoAdjust(const MeshCore::AABBox& bbox) { adjust(bbox); }

  void Camera::enableMovement(bool isEnabled) { mIsMovementEnabled = isEnabled; }

  void Camera::adjust(
    PROJECTION_TYPE projectionType, const MeshCore::AABBox& sceneBBox, float fov
  )
  {
    if (projectionType == PROJECTION_TYPE::PERSPECTIVE)
    {
      perspectiveAdjust(sceneBBox, fov);
    }
    else { orthoAdjust(sceneBBox); }
  }

  void Camera::addOnCameraPosChangedCallback(
    const std::function<cameraPosChangedCallback>& callback
  )
  {
    mCameraPosChangedCallbacks.addCallback(callback);
  }
}  // namespace RenderSystem
