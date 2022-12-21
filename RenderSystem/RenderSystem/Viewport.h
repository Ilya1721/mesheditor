#pragma once

#include <glm/glm.hpp>

#include "Camera.h"

namespace RenderSystem
{
	enum class PROJECTION_TYPE
	{
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	class Viewport
	{
	public:
		Viewport(int x, int y, int width, int height) noexcept;

		Camera& getCamera() noexcept;

		glm::mat4 calcProjectionMatrix(PROJECTION_TYPE projectionType) noexcept;

		float getFov() const noexcept;
		void setFov(float fov) noexcept;

		float getNearPlaneDistance() const noexcept;
		void setNearPlaneDistance(float nearPlaneDistance) noexcept;

		float getFarPlaneDistance() const noexcept;
		void setFarPlaneDistance(float farPlaneDisance) noexcept;

		void setViewport(int x, int y, int width, int height) noexcept;

		int getX() const noexcept;
		void setX(int x) noexcept;

		int getY() const noexcept;
		void setY(int y) noexcept;

		int getWidth() const noexcept;
		void setWidth(int width) noexcept;

		int getHeight() const noexcept;
		void setHeight(int height) noexcept;

	private:
		float mFov;
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		int mX;
		int mY;

		int mWidth;
		int mHeight;
		
		Camera mCamera;
	};
}

