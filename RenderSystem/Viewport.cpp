#include "pch.h"

#include "Viewport.h"

namespace RenderSystem
{
	Viewport::Viewport(int x, int y, int width, int height) noexcept :
		mFov(45.0f),
		mNearPlaneDistance(0.01f),
		mFarPlaneDistance(1000.0f),
		mPos(x, y),
		mWidth(800),
		mHeight(600),
		mProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC)
	{}

	Camera& Viewport::getCamera() noexcept
	{
		return mCamera;
	}

	glm::mat4 Viewport::calcProjectionMatrix() noexcept
	{
		if (mProjectionType == PROJECTION_TYPE::ORTHOGRAPHIC)
		{
			return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, mNearPlaneDistance, mFarPlaneDistance);
		}
		else
		{
			return glm::perspective(mFov, static_cast<float>(mWidth / mHeight), mNearPlaneDistance, mFarPlaneDistance);
		}
	}

	PROJECTION_TYPE Viewport::getProjectionType() const noexcept
	{
		return mProjectionType;
	}

	void Viewport::setProjectionType(PROJECTION_TYPE projectionType) noexcept
	{
		mProjectionType = projectionType;
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
		setPos(x, y);
		mWidth = width;
		mHeight = height;
	}

	const glm::ivec2& Viewport::getPos() const noexcept
	{
		return mPos;
	}

	void Viewport::setPos(int x, int y) noexcept
	{
		mPos.x = x;
		mPos.y = y;
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
