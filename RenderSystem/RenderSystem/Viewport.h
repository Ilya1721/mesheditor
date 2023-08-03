#pragma once

#include "GeometryCore/Matrix.h"
#include "GeometryCore/Vector.h"

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

		const Geometry::Matrix4D& getProjectionMatrix() const;
		PROJECTION_TYPE getProjectionType() const;
		void setProjectionType(PROJECTION_TYPE projectionType);

		float getFov() const;
		float getNearPlaneDistance() const;
		float getFarPlaneDistance() const;
		int getWidth() const;
		int getHeight() const;
		const Geometry::Vector2D& getPos() const;

		void resize(int width, int height);

	private:
		void init();
		Geometry::Matrix4D createProjectionMatrix() const;

	private:
		float mFov;
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		Geometry::Vector2D mPos;
		int mWidth;
		int mHeight;
		
		PROJECTION_TYPE mProjectionType;
		Geometry::Matrix4D mProjectionMatrix;
	};
}

