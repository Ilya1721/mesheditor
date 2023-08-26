#include "Viewport.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Constants.h"

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
		glViewport(static_cast<int>(mPos.x), static_cast<int>(mPos.y), mWidth, mHeight);
		mProjectionMatrix = createProjectionMatrix();
	}

	glm::mat4 Viewport::createProjectionMatrix() const
	{
		auto width = static_cast<float>(mWidth);
		auto height = static_cast<float>(mHeight);

		if (mProjectionType == PROJECTION_TYPE::ORTHOGRAPHIC)
		{
			return glm::ortho(0.0f, width, 0.0f, height, mNearPlaneDistance, mFarPlaneDistance); // need to implement later
		}

		return glm::perspective(glm::radians(mFov), width / height, mNearPlaneDistance, mFarPlaneDistance);
	}

	const glm::mat4& Viewport::getProjectionMatrix() const
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

	const glm::vec2& Viewport::getPos() const
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
