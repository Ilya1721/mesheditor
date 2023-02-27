#include "pch.h"

#include "Viewport.h"

#include "Utility/GeomUtils.h"

namespace RenderSystem
{
	Viewport::Viewport(int x, int y, int width, int height) noexcept :
		mFov(45.0f),
		mNearPlaneDistance(0.01f),
		mFarPlaneDistance(1000.0f),
		mPos(x, y),
		mWidth(800),
		mHeight(600),
		mProjectionType(PROJECTION_TYPE::PERSPECTIVE),
		mProjectionMatrix(1.0f)
	{}

	Camera& Viewport::getCamera() noexcept
	{
		return mCamera;
	}

	glm::mat4 Viewport::getProjectionMatrix() const noexcept
	{
		return mProjectionMatrix;
	}

	void Viewport::calcProjectionMatrix() noexcept
	{
		if (mProjectionType == PROJECTION_TYPE::ORTHOGRAPHIC)
		{
			mProjectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, mNearPlaneDistance, mFarPlaneDistance);
		}
		else
		{
			mProjectionMatrix = glm::perspective(mFov, static_cast<float>(mWidth / mHeight), mNearPlaneDistance, mFarPlaneDistance);
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

	void Viewport::adjustToObject(const MeshCore::Object3D& object) noexcept
	{
		MeshCore::AABBox bbox;
		bbox.setFromObject(object);

		auto bboxCenter = bbox.getCenter();
		auto bboxHalfHeight = bbox.getHeight() * 0.5;
		auto distanceToCamera = bboxHalfHeight / std::tan(glm::radians(mFov * 0.5));

		mCamera.setPositionTargetUp({ bboxCenter.x - 18.0f, bboxCenter.y, distanceToCamera }, bboxCenter, { 0.0f, 1.0f, 0.0f });
		mCamera.calcViewMatrix();
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

	glm::vec3 Viewport::screenToWorld(const glm::vec3& screenPos) const noexcept
	{
		auto modelViewInverse = glm::inverse(mCamera.getViewMatrix() * getProjectionMatrix());

		const float newMin = -1.0f;
		const float newMax = 1.0f;

		auto x = Utility::GeomUtils::convertToRange(screenPos.x, 0.0f, static_cast<float>(mWidth), newMin, newMax);
		auto y = Utility::GeomUtils::convertToRange(screenPos.y, 0.0f, static_cast<float>(mHeight), newMin, newMax);
		auto z = Utility::GeomUtils::convertToRange(screenPos.z, mNearPlaneDistance, mFarPlaneDistance, newMin, newMax);
		
		return glm::vec4(x, y, z, 1.0f) * modelViewInverse;
	}
}
