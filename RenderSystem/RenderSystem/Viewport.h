#pragma once

#include <glm/glm.hpp>

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
		Viewport(int x, int y, int width, int height);

		const glm::mat4& getProjectionMatrix() const;
		void setProjectionType(PROJECTION_TYPE projectionType);

		float getFov() const;
		float getNearPlaneDistance() const;
		float getFarPlaneDistance() const;
		float getWidth() const;
		float getHeight() const;
		const glm::ivec2& getPos() const;

		void resize(int width, int height);

	private:
		void setGLViewport();
		glm::mat4 createProjectionMatrix() const;
		void updateProjectionMatrix();

	private:
		float mFov;
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		glm::ivec2 mPos;
		glm::vec2 mDimensions;
		
		PROJECTION_TYPE mProjectionType;
		glm::mat4 mProjectionMatrix;
	};
}

