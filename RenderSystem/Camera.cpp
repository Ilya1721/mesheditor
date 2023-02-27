#include "pch.h"

#include "Camera.h"

#include "Utility/EqualityUtils.h"

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

	glm::mat4 Camera::getViewMatrix() const noexcept
	{
		return mViewMatrix;
	}

	void Camera::calcViewMatrix() noexcept
	{
		mViewMatrix = glm::lookAt(mPosition, mTarget, mUp);
	}

	void Camera::setTarget(const glm::vec3& newTarget) noexcept
	{
		setPositionTargetUp(mPosition, newTarget, mUp);
	}

	const glm::vec3& Camera::getTarget() const noexcept
	{
		return mTarget;
	}

	void Camera::setPosition(const glm::vec3& newPosition) noexcept
	{
		setPositionTargetUp(newPosition, mTarget, mUp);
	}

	const glm::vec3& Camera::getPosition() const noexcept
	{
		return mPosition;
	}

	void Camera::setUp(const glm::vec3& newUp) noexcept
	{
		setPositionTargetUp(mPosition, mTarget, newUp);
	}

	const glm::vec3& Camera::getUp() const noexcept
	{
		return mUp;
	}

	void Camera::setPositionTargetUp(const glm::vec3& newPosition, const glm::vec3& newTarget, const glm::vec3& newUp) noexcept
	{
		if (Utility::EqualityUtils::isEqual(newUp, glm::vec3(0.0f, 0.0f, 0.0f)))
		{
			std::cerr << "The Camera Up vector must never be equal to null vector" << std::endl;
			return;
		}

		if (Utility::EqualityUtils::isEqual(mTarget, newPosition))
		{
			std::cerr << "Target must never be equal to the position of the Camera" << std::endl;
			return;
		}

		mPosition = newPosition;
		mTarget = newTarget;
		mUp = newUp;

		recalcCamera();
	}

	void Camera::translate(const glm::vec3& movement) noexcept
	{
		mPosition += movement;
		mTarget += movement;
	}

	void Camera::recalcCamera() noexcept
	{
		mDirection = glm::normalize(mTarget - mPosition);

		if (Utility::EqualityUtils::isEqual(mDirection, mUp) || !Utility::EqualityUtils::isEqual(glm::dot(mDirection, mUp), 0.0f))
		{
			std::cerr << "Camera Up and Direction vectors must be perpendicular to each other" << std::endl;
			return;
		}

		mRight = glm::normalize(glm::cross(mUp, mDirection));
	}
}
