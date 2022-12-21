#include "pch.h"

#include "Viewport.h"

namespace RenderSystem
{
	Viewport::Viewport(int x, int y, int width, int height) noexcept 
		:
		mFov(45.0f),
		mNearPlaneDistance(0.01f),
		mFarPlaneDistance(1000.0f),
		mX(x),
		mY(y),
		mWidth(800),
		mHeight(600)
	{}

	Camera& Viewport::getCamera() noexcept
	{
		return mCamera;
	}

	glm::mat4 Viewport::calcProjectionMatrix(PROJECTION_TYPE projectionType) noexcept
	{
		if (projectionType == PROJECTION_TYPE::ORTHOGRAPHIC)
		{
			return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, mNearPlaneDistance, mFarPlaneDistance);
		}
		else
		{
			return glm::perspective(mFov, static_cast<float>(mWidth / mHeight), mNearPlaneDistance, mFarPlaneDistance);
		}
	}

	float Viewport::getFov() const noexcept
	{
		return mFov;
	}

	void Viewport::setFov(float fov) noexcept
	{
		mFov = fov;
	}

	float Viewport::getNearPlaneDistance() const noexcept
	{
		return mNearPlaneDistance;
	}

	void Viewport::setNearPlaneDistance(float nearPlaneDistance) noexcept
	{
		mNearPlaneDistance = nearPlaneDistance;
	}

	float Viewport::getFarPlaneDistance() const noexcept
	{
		return mFarPlaneDistance;
	}

	void Viewport::setFarPlaneDistance(float farPlaneDistance) noexcept
	{
		mFarPlaneDistance = farPlaneDistance;
	}

	void Viewport::setViewport(int x, int y, int width, int height) noexcept
	{
		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;
	}

	int Viewport::getX() const noexcept
	{
		return mX;
	}

	void Viewport::setX(int x) noexcept
	{
		mX = x;
	}

	int Viewport::getY() const noexcept
	{
		return mY;
	}

	void Viewport::setY(int y) noexcept
	{
		mY = y;
	}
		 
	int Viewport::getWidth() const noexcept
	{
		return mWidth;
	}

	void Viewport::setWidth(int width) noexcept
	{
		mWidth = width;
	}

	int Viewport::getHeight() const noexcept
	{
		return mHeight;
	}

	void Viewport::setHeight(int height) noexcept
	{
		mHeight = height;
	}
}
