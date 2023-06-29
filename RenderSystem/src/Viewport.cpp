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
	Viewport::Viewport(float x, float y, int width, int height) :
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
		glViewport(static_cast<int>(mPos.x()), static_cast<int>(mPos.y()), mWidth, mHeight);
		mProjectionMatrix = createProjectionMatrix();
	}

	Geometry::Matrix4D Viewport::createProjectionMatrix() const
	{
		auto width = static_cast<float>(mWidth);
		auto height = static_cast<float>(mHeight);

		if (mProjectionType == PROJECTION_TYPE::ORTHOGRAPHIC)
		{
			return Geometry::Matrix4D::ortho(0.0f, width, 0.0f, height, mNearPlaneDistance, mFarPlaneDistance); // need to implement later
		}

		return Geometry::Matrix4D::perspective(Geometry::toRadians(mFov), width / height, mNearPlaneDistance, mFarPlaneDistance);
	}

	const Geometry::Matrix4D& Viewport::getProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	PROJECTION_TYPE Viewport::getProjectionType() const
	{
		return mProjectionType;
	}

	void Viewport::setProjectionType(PROJECTION_TYPE projectionType)
	{
		mProjectionType = projectionType;
		mProjectionMatrix = createProjectionMatrix();
	}

	float Viewport::getFov() const
	{
		return mFov;
	}

	float Viewport::getNearPlaneDistance() const
	{
		return mNearPlaneDistance;
	}

	float Viewport::getFarPlaneDistance() const
	{
		return mFarPlaneDistance;
	}
		 
	int Viewport::getWidth() const
	{
		return mWidth;
	}

	int Viewport::getHeight() const
	{
		return mHeight;
	}

	const Geometry::Vector2D& Viewport::getPos() const
	{
		return mPos;
	}

	void Viewport::resize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		init();
	}
}
