#pragma once

#include <glm/glm.hpp>

#include "Camera.h"

#include "MeshCore/Object3D.h"

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

		glm::mat4 getProjectionMatrix() const noexcept;
		void calcProjectionMatrix() noexcept;

		PROJECTION_TYPE getProjectionType() const noexcept;
		void setProjectionType(PROJECTION_TYPE projectionType) noexcept;

		float getFov() const noexcept;
		void setFov(float fov) noexcept;

		float getNearPlaneDistance() const noexcept;
		void setNearPlaneDistance(float nearPlaneDistance) noexcept;

		float getFarPlaneDistance() const noexcept;
		void setFarPlaneDistance(float farPlaneDisance) noexcept;

		void setViewport(int x, int y, int width, int height) noexcept;

		void adjustToObject(const MeshCore::Object3D& object) noexcept;

		const glm::ivec2& getPos() const noexcept;
		void setPos(int x, int y) noexcept;

		int getWidth() const noexcept;
		void setWidth(int width) noexcept;

		int getHeight() const noexcept;
		void setHeight(int height) noexcept;

	private:
		float mFov;
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		glm::ivec2 mPos;

		int mWidth;
		int mHeight;
		
		Camera mCamera;

		PROJECTION_TYPE mProjectionType;
		glm::mat4 mProjectionMatrix;
	};
}

