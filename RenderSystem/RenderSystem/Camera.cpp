#include "Camera.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>

#include "GeometryCore/Numeric.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Constants.h"

#include "Constants.h"

using namespace GeometryCore;

namespace RenderSystem
{
	Camera::Camera() :
		mTarget(CAMERA_TARGET),
		mPos(CAMERA_POSITION),
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
		return glm::lookAt(mPos, mTarget, mUp);
	}

	const glm::vec3& Camera::getTarget() const
	{
		return mTarget;
	}

	const glm::vec3& Camera::getPosition() const
	{
		return mPos;
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
		return glm::normalize(mTarget - mPos);
	}

	void Camera::setPositionTargetUp(const glm::vec3& newPosition, const glm::vec3& newTarget, const glm::vec3& newUp)
	{
		if (newUp == glm::vec3(0.0f, 0.0f, 0.0f))
		{
			throw std::exception("The Camera Up vector must never be equal to null vector");
		}

		if (mTarget == newPosition)
		{
			throw std::exception("Target must never be equal to the position of the Camera");
		}

		mPos = newPosition;
		mTarget = newTarget;
		mUp = newUp;
		mRight = calcRight();
		mViewMatrix = createViewMatrix();
	}

	void Camera::pan(const glm::vec3& firstPoint, const glm::vec3& secondPoint)
	{
		Ray firstRay(firstPoint, firstPoint - mPos);
		Ray secondRay(secondPoint, secondPoint - mPos);
		auto targetPlane = getTargetPlane();
		auto firstIntersectionPoint = firstRay.findIntersection(targetPlane);
		auto secondIntersectionPoint = secondRay.findIntersection(targetPlane);
		auto movement = firstIntersectionPoint - secondIntersectionPoint;
		translate(movement);
		mViewMatrix = createViewMatrix();
	}

	void Camera::zoomToPoint(const glm::vec3& unProjectedMousePos, int scrollSign)
	{
		Ray zoomRay(mPos, unProjectedMousePos - mPos);
		auto targetPlane = getTargetPlane();
		auto intersectionPoint = zoomRay.findIntersection(targetPlane);
		mTarget = intersectionPoint;
		auto direction = mTarget - mPos;
		mPos += direction * static_cast<float>(scrollSign) * ZOOM_STEP_KOEF;

		glm::vec3 xozDirection(direction.x, 0.0, direction.z);
		auto rotateRightVecAngle = HALF_PI - glm::angle(xozDirection, mRight);
		mRight = glm::rotate(glm::mat4(1.0f), rotateRightVecAngle, mUp) * glm::normalize(glm::vec4(mRight, 0.0f));
		mUp = glm::cross(glm::normalize(direction), mRight);
		mViewMatrix = createViewMatrix();
	}

	void Camera::translate(const glm::vec3& movement)
	{
		mPos += movement;
		mTarget += movement;
	}

	Plane Camera::getTargetPlane() const
	{
		return { mTarget, mPos - mTarget };
	}

	glm::vec3 Camera::calcRight()
	{
		auto direction = glm::normalize(mTarget - mPos);
		if (!isEqual(glm::dot(direction, mUp), 0.0f))
		{
			throw std::exception("Camera Up and Direction vectors must be perpendicular to each other");
		}

		return glm::normalize(glm::cross(mUp, direction));
	}

	void Camera::adjust(const MeshCore::AABBox& bbox, float fov)
	{
		auto distanceToCamera = bbox.getHeight() / (2.0f * glm::tan(glm::radians(fov / 2.0f)));
		auto bboxCenter = bbox.getCenter();
		glm::vec3 position(bboxCenter.x, bboxCenter.y, bbox.getMax().z + distanceToCamera * CAMERA_DIST_TO_BBOX_KOEF);
		setPositionTargetUp(position, bboxCenter, mUp);
	}
}
