module;
#include "GeometryCore/Vector.h"
#include "GeometryCore/Matrix.h"
#include "GeometryCore/Numeric.h"
module Camera;

import std;

using namespace Geometry;

namespace RenderSystem
{
	Camera::Camera() noexcept :
		mTarget(0.0f, 0.0f, 0.0f),
		mPosition(0.0f, 0.0f, 1.0f),
		mDirection(mTarget - mPosition),
		mUp(0.0f, 1.0f, 0.0f),
		mRight(1.0f, 0.0f, 0.0f),
		mViewMatrix(1.0f)
	{}

	Matrix4D Camera::getViewMatrix() const noexcept
	{
		return mViewMatrix;
	}

	void Camera::calcViewMatrix() noexcept
	{
		mViewMatrix = Matrix4D::lookAt(mPosition, mTarget, mUp);
	}

	void Camera::setTarget(const Vector3D& newTarget) noexcept
	{
		setPositionTargetUp(mPosition, newTarget, mUp);
	}

	const Vector3D& Camera::getTarget() const noexcept
	{
		return mTarget;
	}

	void Camera::setPosition(const Vector3D& newPosition) noexcept
	{
		setPositionTargetUp(newPosition, mTarget, mUp);
	}

	const Vector3D& Camera::getPosition() const noexcept
	{
		return mPosition;
	}

	void Camera::setUp(const Vector3D& newUp) noexcept
	{
		setPositionTargetUp(mPosition, mTarget, newUp);
	}

	const Vector3D& Camera::getUp() const noexcept
	{
		return mUp;
	}

	void Camera::setPositionTargetUp(const Vector3D& newPosition, const Vector3D& newTarget, const Vector3D& newUp) noexcept
	{
		if (newUp == Vector3D(0.0f, 0.0f, 0.0f))
		{
			std::cerr << "The Camera Up vector must never be equal to null vector" << std::endl;
			return;
		}

		if (mTarget == newPosition)
		{
			std::cerr << "Target must never be equal to the position of the Camera" << std::endl;
			return;
		}

		mPosition = newPosition;
		mTarget = newTarget;
		mUp = newUp;

		recalcCamera();
	}

	void Camera::translate(const Vector3D& movement) noexcept
	{
		mPosition += movement;
		mTarget += movement;
	}

	void Camera::recalcCamera() noexcept
	{
		mDirection = Vector3D::normalize(mTarget - mPosition);

		if (mDirection == mUp || !isEqual(Vector3D::dot(mDirection, mUp), 0.0))
		{
			std::cerr << "Camera Up and Direction vectors must be perpendicular to each other" << std::endl;
			return;
		}

		mRight = Vector3D::normalize(Vector3D::cross(mUp, mDirection));
	}
}
