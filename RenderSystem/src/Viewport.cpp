module;
#include "GeometryCore/Numeric.h"
module Viewport;

import AABBox;

namespace RenderSystem
{
	Viewport::Viewport(int x, int y, int width, int height) noexcept :
		mFov(45.0),
		mNearPlaneDistance(0.01),
		mFarPlaneDistance(1000.0),
		mPos(x, y),
		mWidth(800),
		mHeight(600),
		mProjectionType(PROJECTION_TYPE::PERSPECTIVE),
		mProjectionMatrix(1.0)
	{}

	Camera& Viewport::getCamera() noexcept
	{
		return mCamera;
	}

	Geometry::Matrix4D Viewport::getProjectionMatrix() const noexcept
	{
		return mProjectionMatrix;
	}

	void Viewport::calcProjectionMatrix() noexcept
	{
		if (mProjectionType == PROJECTION_TYPE::ORTHOGRAPHIC)
		{
			mProjectionMatrix = Geometry::Matrix4D::ortho(-1.0, 1.0, -1.0, 1.0, mNearPlaneDistance, mFarPlaneDistance);
		}
		else
		{
			mProjectionMatrix = Geometry::Matrix4D::perspective(mFov, static_cast<double>(mWidth / mHeight), mNearPlaneDistance, mFarPlaneDistance);
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

	double Viewport::getFov() const noexcept
	{
		return mFov;
	}

	void Viewport::setFov(double fov) noexcept
	{
		mFov = fov;
	}

	double Viewport::getNearPlaneDistance() const noexcept
	{
		return mNearPlaneDistance;
	}

	void Viewport::setNearPlaneDistance(double nearPlaneDistance) noexcept
	{
		mNearPlaneDistance = nearPlaneDistance;
	}

	double Viewport::getFarPlaneDistance() const noexcept
	{
		return mFarPlaneDistance;
	}

	void Viewport::setFarPlaneDistance(double farPlaneDistance) noexcept
	{
		mFarPlaneDistance = farPlaneDistance;
	}

	void Viewport::setViewport(int x, int y, int width, int height) noexcept
	{
		setPos(x, y);
		mWidth = width;
		mHeight = height;
	}

	void Viewport::adjustToObject(const MeshCore::Object3D& object) noexcept
	{
		MeshCore::AABBox bbox;
		bbox.setFromObject(object);

		auto bboxCenter = bbox.getCenter();
		auto bboxHalfHeight = bbox.getHeight() * 0.5;
		auto distanceToCamera = bboxHalfHeight / Geometry::tan(Geometry::toRadians(mFov * 0.5));
		double bboxXOffset = 18.0;

		mCamera.setPositionTargetUp({ bboxCenter.x() - bboxXOffset, bboxCenter.y(), distanceToCamera},
									  bboxCenter.getVec3(), {0.0, 1.0, 0.0});
		mCamera.calcViewMatrix();
	}

	const Geometry::Vector2D& Viewport::getPos() const noexcept
	{
		return mPos;
	}

	void Viewport::setPos(int x, int y) noexcept
	{
		mPos.setX(x);
		mPos.setY(y);
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
