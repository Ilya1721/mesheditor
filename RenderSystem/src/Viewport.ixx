module;
#include "GeometryCore/Matrix.h"
#include "GeometryCore/Vector.h"
export module Viewport;

export namespace RenderSystem
{
	enum class PROJECTION_TYPE
	{
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	class Viewport final
	{
	public:
		Viewport(float x, float y, int width, int height) noexcept;

		const Geometry::Matrix4D& getProjectionMatrix() const noexcept;
		PROJECTION_TYPE getProjectionType() const noexcept;
		void setProjectionType(PROJECTION_TYPE projectionType) noexcept;

		float getFov() const noexcept;
		float getNearPlaneDistance() const noexcept;
		float getFarPlaneDistance() const noexcept;

		int getWidth() const noexcept;
		int getHeight() const noexcept;
		void setWidth(int width) noexcept;
		void setHeight(int height) noexcept;

	private:
		void init();
		Geometry::Matrix4D createProjectionMatrix() const noexcept;

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

