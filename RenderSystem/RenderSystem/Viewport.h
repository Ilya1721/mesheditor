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

		void setProjectionType(PROJECTION_TYPE projectionType);
		void resize(int width, int height);

		float getFov() const;
		float getNearPlaneDistance() const;
		float getFarPlaneDistance() const;
		float getWidth() const;
		float getHeight() const;
		const glm::ivec2& getPos() const;
		const glm::mat4& getProjectionMatrix() const;

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

