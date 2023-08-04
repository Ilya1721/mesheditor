#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
	enum class PROJECTION_TYPE
	{
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	class Viewport final
	{
	public:
		Viewport(float x, float y, int width, int height);

		const glm::mat4& getProjectionMatrix() const;
		PROJECTION_TYPE getProjectionType() const;
		void setProjectionType(PROJECTION_TYPE projectionType);

		float getFov() const;
		float getNearPlaneDistance() const;
		float getFarPlaneDistance() const;
		int getWidth() const;
		int getHeight() const;
		const glm::vec2& getPos() const;

		void resize(int width, int height);

	private:
		void init();
		glm::mat4 createProjectionMatrix() const;

	private:
		float mFov;
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		glm::vec2 mPos;
		int mWidth;
		int mHeight;
		
		PROJECTION_TYPE mProjectionType;
		glm::mat4 mProjectionMatrix;
	};
}

