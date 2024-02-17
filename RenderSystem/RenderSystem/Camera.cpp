#include "Camera.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/epsilon.hpp>

#include "Constants.h"

using namespace GeometryCore;

namespace Utils
{
	using namespace RenderSystem;

	static glm::mat4 createViewMatrix(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
	{
		return glm::lookAt(eye, target, up);
	}
}

namespace RenderSystem
{
	Camera::Camera() :
		mTarget(CAMERA_TARGET),
		mEye(CAMERA_POSITION),
		mUp(CAMERA_UP),
		mRight(CAMERA_RIGHT)
	{
		mViewMatrix = createViewMatrix();
	}

	const glm::mat4& Camera::getViewMatrix() const
	{
		return mViewMatrix;
	}

	glm::mat4 Camera::createViewMatrix() const
	{
		return Utils::createViewMatrix(mEye, mTarget, mUp);
	}

	const glm::vec3& Camera::getTarget() const
	{
		return mTarget;
	}

	const glm::vec3& Camera::getEye() const
	{
		return mEye;
	}

	const glm::vec3& Camera::getUp() const
	{
		return mUp;
	}

	const glm::vec3& Camera::getRight() const
	{
		return mRight;
	}

	glm::vec3 Camera::getNormalizedDirection() const
	{
		return glm::normalize(mTarget - mEye);
	}

	Ray Camera::getCameraRay(const glm::vec3& cursorPosInWorldSpace) const
	{
		return { mEye, cursorPosInWorldSpace - mEye };
	}

	void Camera::setEyeTargetUp(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
	{
		if (up == glm::vec3(0.0f, 0.0f, 0.0f))
		{
			throw std::exception("The Camera Up vector must never be equal to null vector");
		}

		if (mTarget == eye)
		{
			throw std::exception("Target must never be equal to the position of the Camera");
		}

		mEye = eye;
		mTarget = target;
		mUp = glm::normalize(up);
		mRight = calcRight();
		mViewMatrix = createViewMatrix();
	}

	void Camera::pan(const glm::vec3& startPointInWorldSpace, const glm::vec3& endPointInWorldSpace)
	{
		Ray startRay(startPointInWorldSpace, startPointInWorldSpace - mEye);
		Ray endRay(endPointInWorldSpace, endPointInWorldSpace - mEye);
		auto targetPlane = getTargetPlane();
		translate(startRay.findIntersection(targetPlane).value() - endRay.findIntersection(targetPlane).value());
	}

	void Camera::orbit(const glm::vec3& startPointInNDC, const glm::vec3& endPointInNDC)
	{
		auto startPointInNDCWithZ = getCursorPosInNDCWithZ(startPointInNDC);
		auto endPointInNDCWithZ = getCursorPosInNDCWithZ(endPointInNDC);
		auto orbitTransform = getOrbitTransform(startPointInNDCWithZ, endPointInNDCWithZ);

		auto eye = glm::vec3(orbitTransform * glm::vec4(mEye - mTarget, 1.0f));
		auto up = glm::vec3(orbitTransform * glm::vec4(mUp, 0.0f));
		setEyeTargetUp(eye + mTarget, mTarget, up);
	}

	void Camera::zoom(float step)
	{
		mEye += glm::normalize(mTarget - mEye) * step;
		mViewMatrix = createViewMatrix();
	}

	void Camera::translate(const glm::vec3& movement)
	{
		mEye += movement;
		mTarget += movement;
		mViewMatrix = createViewMatrix();
	}

	glm::mat4 Camera::calculateViewMatrixWithTargetAtOrigin() const
	{
		return Utils::createViewMatrix(mEye - mTarget, glm::vec3(0.0, 0.0, 0.0), mUp);
	}

	Plane Camera::getTargetPlane() const
	{
		return { mTarget, mEye - mTarget };
	}

	glm::vec3 Camera::getCursorPosInNDCWithZ(const glm::vec3& cursorPosInNDC) const
	{
		glm::vec3 cursorPosInNDCWithZ = cursorPosInNDC;
		auto lengthSquared = std::powf(cursorPosInNDC.x, 2) + std::powf(cursorPosInNDC.y, 2);
		auto validZ = std::abs(sqrtf(1 - std::powf(cursorPosInNDC.x, 2) - std::powf(cursorPosInNDC.y, 2)));
		cursorPosInNDCWithZ.z = lengthSquared > 1.0 ? 0.0 : validZ;

		return cursorPosInNDCWithZ;
	}

	glm::mat4 Camera::getOrbitTransform(const glm::vec3& startPosInNDCWithZ, const glm::vec3& endPosInNDCWithZ) const
	{
		auto rotationAngle = glm::angle(glm::normalize(startPosInNDCWithZ), glm::normalize(endPosInNDCWithZ));
		auto rotationAxisInNDC = glm::cross(startPosInNDCWithZ, endPosInNDCWithZ);
		auto rotationAxisInCameraSpace = glm::inverse(calculateViewMatrixWithTargetAtOrigin()) * glm::vec4(rotationAxisInNDC, 0.0f);
		auto unitRotationAxisInCameraSpace = glm::normalize(glm::vec3(rotationAxisInCameraSpace));

		return glm::rotate(-rotationAngle * ORBIT_SPEED_KOEF, unitRotationAxisInCameraSpace);
	}

	glm::vec3 Camera::calcRight() const
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
		auto distanceToCamera = bbox.getHeight() / (2.0f * glm::tan(glm::radians(fov / 2.0f)));
		auto bboxCenter = bbox.getCenter();
		glm::vec3 eye(bboxCenter.x, bboxCenter.y, bbox.getMax().z + distanceToCamera * CAMERA_DIST_TO_BBOX_KOEF);
		setEyeTargetUp(eye, bboxCenter, mUp);
	}
}
