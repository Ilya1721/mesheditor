#include "Viewport.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "glad.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Constants.h"

namespace RenderSystem
{
	Viewport::Viewport(int x, int y, int width, int height) :
		mFov(FOV),
		mNearPlaneDistance(NEAR_PLANE_DISTANCE),
		mFarPlaneDistance(FAR_PLANE_DISTANCE),
		mPos(x, y),
		mDimensions(width, height),
		mProjectionType(PROJECTION_TYPE::PERSPECTIVE),
		mProjectionMatrix(1.0f)
	{
		setGLViewport();
	}

	void Viewport::setGLViewport()
	{
		glViewport(mPos.x, mPos.y, mDimensions.x, mDimensions.y);
	}

	glm::mat4 Viewport::createProjectionMatrix() const
	{
		if (mProjectionType == PROJECTION_TYPE::ORTHOGRAPHIC)
		{
			return glm::ortho(0.0f, mDimensions.x, 0.0f, mDimensions.y, mNearPlaneDistance, mFarPlaneDistance); // need to fix
		}

		return glm::perspective(glm::radians(mFov), mDimensions.x / mDimensions.y, mNearPlaneDistance, mFarPlaneDistance);
	}

	void Viewport::updateProjectionMatrix()
	{
		mProjectionMatrix = createProjectionMatrix();
	}

	const glm::mat4& Viewport::getProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	void Viewport::setProjectionType(PROJECTION_TYPE projectionType)
	{
		mProjectionType = projectionType;
		updateProjectionMatrix();
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
		 
	float Viewport::getWidth() const
	{
		return mDimensions.x;
	}

	float Viewport::getHeight() const
	{
		return mDimensions.y;
	}

	const glm::ivec2& Viewport::getPos() const
	{
		return mPos;
	}

	void Viewport::resize(int width, int height)
	{
		mDimensions.x = width;
		mDimensions.y = height;
		setGLViewport();
		updateProjectionMatrix();
	}
}
