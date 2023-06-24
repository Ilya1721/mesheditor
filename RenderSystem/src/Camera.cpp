module;
#include "GeometryCore/Numeric.h"
module Camera;

import <iostream>;

import RenderSystemConsts;

using namespace Geometry;

namespace RenderSystem
{
	Camera::Camera() noexcept :
		mTarget(CAMERA_TARGET),
		mPosition(CAMERA_POSITION),
		mUp(CAMERA_UP),
		mRight(CAMERA_RIGHT),
		mViewMatrix(1.0f)
	{}

	const Matrix4D& Camera::getViewMatrix() const noexcept
	{
		return mViewMatrix;
	}

	Matrix4D Camera::createViewMatrix() const noexcept
	{
		return Matrix4D::lookAt(mPosition, mTarget, mUp);
	}

	void Camera::setTarget(const Vector3D& newTarget)
	{
		setPositionTargetUp(mPosition, newTarget, mUp);
	}

	const Vector3D& Camera::getTarget() const noexcept
	{
		return mTarget;
	}

	void Camera::setPosition(const Vector3D& newPosition)
	{
		setPositionTargetUp(newPosition, mTarget, mUp);
	}

	const Vector3D& Camera::getPosition() const noexcept
	{
		return mPosition;
	}

	void Camera::setUp(const Vector3D& newUp)
	{
		setPositionTargetUp(mPosition, mTarget, newUp);
	}

	const Vector3D& Camera::getUp() const noexcept
	{
		return mUp;
	}

	void Camera::setPositionTargetUp(const Vector3D& newPosition, const Vector3D& newTarget, const Vector3D& newUp)
	{
		if (newUp == Vector3D(0.0f, 0.0f, 0.0f))
		{
			throw std::exception("The Camera Up vector must never be equal to null vector");
		}

		if (mTarget == newPosition)
		{
			throw std::exception("Target must never be equal to the position of the Camera");
		}

		mPosition = newPosition;
		mTarget = newTarget;
		mUp = newUp;
		mRight = calcRight();
	}

	void Camera::translate(const Vector3D& movement) noexcept
	{
		mPosition += movement;
		mTarget += movement;
	}

	Geometry::Vector3D Camera::calcRight()
	{
		auto direction = Vector3D::normalize(mTarget - mPosition);
		if (!isEqual(Vector3D::dot(direction, mUp), 0.0f))
		{
			throw std::exception("Camera Up and Direction vectors must be perpendicular to each other");
		}

		return Vector3D::normalize(Vector3D::cross(mUp, direction));
	}

	void Camera::adjust(const MeshCore::AABBox& bbox, float fov) noexcept
	{
		auto bboxCenter = bbox.getCenter();
		auto bboxHalfHeight = bbox.getHeight() * 0.5f;
		auto distanceToCamera = bboxHalfHeight / Geometry::tan(Geometry::toRadians(fov * 0.5f));

		setPositionTargetUp({ bboxCenter.x(), bboxCenter.y(), distanceToCamera }, bboxCenter.getVec3(), mUp);
	}
}
