#include "pch.h"

#include "Camera.h"

#include "BaseUtility/Utility.h"

namespace RenderSystem
{
	Camera::Camera() noexcept
		:
		mTarget(0.0f, 0.0f, 0.0f),
		mPosition(0.0f, 0.0f, 1.0f),
		mDirection(mTarget - mPosition),
		mUp(0.0f, 1.0f, 0.0f),
		mRight(1.0f, 0.0f, 0.0f)
	{}

	glm::mat4 Camera::calcViewMatrix() noexcept
	{
		return glm::lookAt(mPosition, mTarget, mUp);
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
		if (BaseUtility::Utility::isEqual(newUp, glm::vec3(0.0f, 0.0f, 0.0f)))
		{
			std::cerr << "The Camera Up vector must never be equal to null vector" << std::endl;
			return;
		}

		if (BaseUtility::Utility::isEqual(mTarget, newPosition))
		{
			std::cerr << "Target must never be equal to the position of the Camera" << std::endl;
			return;
		}

		mPosition = newPosition;
		mTarget = newTarget;
		mUp = newUp;

		recalcCamera();
	}

	void Camera::recalcCamera() noexcept
	{
		mDirection = glm::normalize(mTarget - mPosition);

		if (BaseUtility::Utility::isEqual(mDirection, mUp) || !BaseUtility::Utility::isEqual(glm::dot(mDirection, mUp), 0.0f))
		{
			std::cerr << "Camera Up and Direction vectors must be perpendicular to each other" << std::endl;
			return;
		}

		mRight = glm::normalize(glm::cross(mUp, mDirection));
	}
}
