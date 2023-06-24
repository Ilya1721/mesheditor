module;
#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"
#include "GeometryCore/Numeric.h"
module Viewport;

import RenderSystemConsts;

namespace RenderSystem
{
	Viewport::Viewport(float x, float y, int width, int height) noexcept :
		mFov(FOV),
		mNearPlaneDistance(NEAR_PLANE_DISTANCE),
		mFarPlaneDistance(FAR_PLANE_DISTANCE),
		mPos(x, y),
		mWidth(width),
		mHeight(height),
		mProjectionType(PROJECTION_TYPE::PERSPECTIVE),
		mProjectionMatrix(1.0f)
	{
		init();
	}

	void Viewport::init()
	{
		glViewport(static_cast<int>(mPos.x()), static_cast<int>(mPos.y()),
			static_cast<int>(mWidth), static_cast<int>(mHeight));
		mProjectionMatrix = createProjectionMatrix();
	}

	Geometry::Matrix4D Viewport::createProjectionMatrix() const noexcept
	{
		if (mProjectionType == PROJECTION_TYPE::ORTHOGRAPHIC)
		{
			return Geometry::Matrix4D::ortho(-1.0f, 1.0f, -1.0f, 1.0f, mNearPlaneDistance, mFarPlaneDistance);
		}

		return Geometry::Matrix4D::perspective(mFov, static_cast<float>(mWidth / mHeight), mNearPlaneDistance, mFarPlaneDistance);
	}

	const Geometry::Matrix4D& Viewport::getProjectionMatrix() const noexcept
	{
		return mProjectionMatrix;
	}

	PROJECTION_TYPE Viewport::getProjectionType() const noexcept
	{
		return mProjectionType;
	}

	void Viewport::setProjectionType(PROJECTION_TYPE projectionType) noexcept
	{
		mProjectionType = projectionType;
		mProjectionMatrix = createProjectionMatrix();
	}

	float Viewport::getFov() const noexcept
	{
		return mFov;
	}

	float Viewport::getNearPlaneDistance() const noexcept
	{
		return mNearPlaneDistance;
	}

	float Viewport::getFarPlaneDistance() const noexcept
	{
		return mFarPlaneDistance;
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
