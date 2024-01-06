#include "Camera.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/epsilon.hpp>

#include "GeometryCore/Ray.h"
#include "GeometryCore/Constants.h"

#include "Constants.h"

using namespace GeometryCore;

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
		return glm::lookAt(mEye, mTarget, mUp);
	}

	const glm::vec3& Camera::getTarget() const
	{
		return mTarget;
	}

	const glm::vec3& Camera::getPosition() const
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

		mEye = newPosition;
		mTarget = newTarget;
		mUp = newUp;
		mRight = calcRight();
		mViewMatrix = createViewMatrix();
	}

	void Camera::pan(const glm::vec3& unProjectedStartPoint, const glm::vec3& unProjectedEndPoint)
	{
		Ray startRay(unProjectedStartPoint, unProjectedStartPoint - mEye);
		Ray endRay(unProjectedEndPoint, unProjectedEndPoint - mEye);
		translate(startRay.findIntersection(getTargetPlane()) - endRay.findIntersection(getTargetPlane()));
	}

	void Camera::orbit(const glm::vec3& unProjectedStartPoint, const glm::vec3& unProjectedEndPoint)
	{
		auto firstArcballPoint = getPointOnArcball(unProjectedStartPoint);
		auto secondArcballPoint = getPointOnArcball(unProjectedEndPoint);
		auto rotationAngle = glm::angle(firstArcballPoint, secondArcballPoint) * glm::length(mTarget - mEye) * 0.0075f;
		auto rotationAxis = glm::cross(firstArcballPoint, secondArcballPoint);
		auto rotationMatrix = glm::inverse(glm::rotate(glm::mat4(1.0f), rotationAngle, rotationAxis));
		setPositionTargetUp(rotationMatrix * glm::vec4(mEye, 1.0f), rotationMatrix * glm::vec4(mTarget, 1.0f), rotationMatrix * glm::vec4(mUp, 1.0f));
	}

	void Camera::zoomToPoint(const glm::vec3& unProjectedCursorPos, float step)
	{
		translate(glm::normalize(unProjectedCursorPos - mEye) * step);
	}

	void Camera::translate(const glm::vec3& movement)
	{
		mEye += movement;
		mTarget += movement;
		mViewMatrix = createViewMatrix();
	}

	Plane Camera::getTargetPlane() const
	{
		return { mTarget, mEye - mTarget };
	}

	glm::vec3 Camera::getPointOnArcball(const glm::vec3& mousePosNDC) const
	{
		auto trasnformedMousePos = glm::inverse(mViewMatrix) * glm::vec4(mousePosNDC, 1.0f);
		auto lengthSquared = glm::length2(trasnformedMousePos);
		if (lengthSquared > 1.0f)
		{
			return glm::normalize(trasnformedMousePos);
		}

		return glm::vec3(trasnformedMousePos.x, trasnformedMousePos.y, sqrt(1.0f - lengthSquared));
	}

	glm::vec3 Camera::calcRight() const
	{
		auto direction = glm::normalize(mTarget - mEye);
		if (glm::epsilonNotEqual(glm::dot(direction, mUp), 0.0f, 1e-5f))
		{
			throw std::exception("Camera Up and Direction vectors must be perpendicular to each other");
		}

		return glm::normalize(glm::cross(direction, mUp));
	}

	void Camera::adjust(const MeshCore::AABBox& bbox, float fov)
	{
		auto distanceToCamera = bbox.getHeight() / (2.0f * glm::tan(glm::radians(fov / 2.0f)));
		auto bboxCenter = bbox.getCenter();
		glm::vec3 position(bboxCenter.x, bboxCenter.y, bbox.getMax().z + distanceToCamera * CAMERA_DIST_TO_BBOX_KOEF);
		setPositionTargetUp(position, bboxCenter, mUp);
	}
}
