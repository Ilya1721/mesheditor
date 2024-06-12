#include "Camera.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GeometryCore/Ray.h"
#include "GeometryCore/Plane.h"
#include "GeometryCore/Numeric.h"
#include "MeshCore/AABBox.h"

#include "Constants.h"

using namespace GeometryCore;

namespace RenderSystem
{
	Camera::Camera(ShaderTransformationSystem* shaderTransformationSystem) :
		mTarget(CAMERA_TARGET),
		mEye(CAMERA_POSITION),
		mUp(CAMERA_UP),
		mRight(CAMERA_RIGHT),
		mShaderTransformationSystem(shaderTransformationSystem)
	{
		invokeEditOperation([]() {});
	}

	const glm::mat4& Camera::getViewMatrix() const
	{
		return mViewMatrix;
	}

	glm::mat4 Camera::createViewMatrix() const
	{
		return glm::lookAt(mEye, mTarget, mUp);
	}

	const Point3D& Camera::getTarget() const
	{
		return mTarget;
	}

	const Point3D& Camera::getEye() const
	{
		return mEye;
	}

	const Vector3D& Camera::getUp() const
	{
		return mUp;
	}

	const Vector3D& Camera::getRight() const
	{
		return mRight;
	}

	Vector3D Camera::getNormalizedDirection() const
	{
		return glm::normalize(mTarget - mEye);
	}

	Point3D Camera::projectToTargetPlane(const Point3D& cursorPosInWorldSpace) const
	{
		Ray cursorRay(cursorPosInWorldSpace, cursorPosInWorldSpace - mEye);
		return getTargetPlane().findIntersection(cursorRay).value();
	}

	Ray Camera::getCameraRay(const Point3D& cursorPosInWorldSpace) const
	{
		return { mEye, cursorPosInWorldSpace - mEye };
	}

	void Camera::invokeEditOperation(const std::function<void()>& action)
	{
		action();
		mViewMatrix = createViewMatrix();
		mShaderTransformationSystem->setViewModel(glm::value_ptr(mViewMatrix));
	}

	void Camera::setEyeTargetUp(const Point3D& eye, const Point3D& target, const Vector3D& up)
	{
		mEye = eye;
		mTarget = target;
		mUp = glm::normalize(up);
		validateCamera();
		mRight = calcRight();
	}

	void Camera::pan(const Point3D& startPointInWorldSpace, const Point3D& endPointInWorldSpace)
	{
		invokeEditOperation([&startPointInWorldSpace, &endPointInWorldSpace, this]() {
			translate(projectToTargetPlane(startPointInWorldSpace) - projectToTargetPlane(endPointInWorldSpace));
		});
	}

	void Camera::orbit(const Point3D& startPointInNDC, const Point3D& endPointInNDC)
	{
		invokeEditOperation([&startPointInNDC, &endPointInNDC, this]() {
			auto startPointInNDCWithZ = getCursorPosInNDCWithZ(startPointInNDC);
			auto endPointInNDCWithZ = getCursorPosInNDCWithZ(endPointInNDC);
			auto orbitTransform = getOrbitTransform(startPointInNDCWithZ, endPointInNDCWithZ);
			auto eye = Point3D(orbitTransform * glm::vec4(mEye - mTarget, 1.0f));
			auto up = Vector3D(orbitTransform * glm::vec4(mUp, 0.0f));
			setEyeTargetUp(eye + mTarget, mTarget, up);
		});
	}

	void Camera::zoom(float step)
	{
		invokeEditOperation([&step, this]() {
			mEye += glm::normalize(mTarget - mEye) * step;
		});
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

	Plane Camera::getTargetPlane() const
	{
		return { mTarget, mEye - mTarget };
	}

	Point3D Camera::getCursorPosInNDCWithZ(const Point3D& cursorPosInNDC) const
	{
		Point3D cursorPosInNDCWithZ = cursorPosInNDC;
		auto lengthSquared = std::powf(cursorPosInNDC.x, 2) + std::powf(cursorPosInNDC.y, 2);
		auto validZ = std::abs(sqrtf(1 - std::powf(cursorPosInNDC.x, 2) - std::powf(cursorPosInNDC.y, 2)));
		cursorPosInNDCWithZ.z = lengthSquared > 1.0 ? 0.0 : validZ;

		return cursorPosInNDCWithZ;
	}

	glm::mat4 Camera::getOrbitTransform(const Point3D& startPosInNDCWithZ, const Point3D& endPosInNDCWithZ) const
	{
		auto rotationAngle = glm::angle(glm::normalize(startPosInNDCWithZ), glm::normalize(endPosInNDCWithZ));
		if (glm::epsilonEqual(rotationAngle, 0.0f, 1e-5f))
		{
			return glm::mat4(1.0f);
		}

		auto rotationAxisInNDC = glm::cross(startPosInNDCWithZ, endPosInNDCWithZ);
		auto rotationAxisInCameraSpace = glm::inverse(calculateViewMatrixWithTargetAtOrigin()) * Vector4D(rotationAxisInNDC, 0.0f);
		auto unitRotationAxisInCameraSpace = glm::normalize(Vector3D(rotationAxisInCameraSpace));

		return glm::rotate(-rotationAngle * ORBIT_SPEED_KOEF, unitRotationAxisInCameraSpace);
	}

	void Camera::validateCamera() const
	{
		Vector3D nullVector(0.0f, 0.0f, 0.0f);
		if (isEqual(mEye, nullVector) || isEqual(mTarget, nullVector) || isEqual(mUp, nullVector) || isEqual(mRight, nullVector))
		{
			throw std::exception("Some important Camera vector is null vector");
		}

		if (glm::any(glm::isnan(mEye)) || glm::any(glm::isnan(mTarget)) || glm::any(glm::isnan(mUp)) || glm::any(glm::isnan(mRight)))
		{
			throw std::exception("Some important Camera vector contains coordinates that are not numbers");
		}
	}

	Vector3D Camera::calcRight() const
	{
		auto directionNormalized = getNormalizedDirection();
		if (glm::epsilonNotEqual(glm::dot(directionNormalized, mUp), 0.0f, 1e-5f))
		{
			throw std::exception("Camera Up and Direction vectors must be perpendicular to each other");
		}

		return glm::cross(directionNormalized, mUp);
	}

	void Camera::adjust(const MeshCore::AABBox& bbox, float fov)
	{
		invokeEditOperation([&bbox, &fov, this]() {
			auto distanceToCamera = bbox.getHeight() / (2.0f * glm::tan(glm::radians(fov / 2.0f)));
			auto bboxCenter = bbox.getCenter();
			Point3D eye(bboxCenter.x, bboxCenter.y, bbox.getMax().z + distanceToCamera * CAMERA_DIST_TO_BBOX_KOEF);
			setEyeTargetUp(eye, bboxCenter, mUp);
		});
	}
}
