module;
#include "GeometryCore/Matrix.h"
#include "GeometryCore/Vector.h"
export module Viewport;

import Camera;
import Object3D;

export namespace RenderSystem
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

		Geometry::Matrix4D getProjectionMatrix() const noexcept;
		void calcProjectionMatrix() noexcept;

		PROJECTION_TYPE getProjectionType() const noexcept;
		void setProjectionType(PROJECTION_TYPE projectionType) noexcept;

		double getFov() const noexcept;
		void setFov(double fov) noexcept;

		double getNearPlaneDistance() const noexcept;
		void setNearPlaneDistance(double nearPlaneDistance) noexcept;

		double getFarPlaneDistance() const noexcept;
		void setFarPlaneDistance(double farPlaneDisance) noexcept;

		void setViewport(int x, int y, int width, int height) noexcept;

		void adjustToObject(const MeshCore::Object3D& object) noexcept;

		const Geometry::Vector2D& getPos() const noexcept;
		void setPos(int x, int y) noexcept;

		int getWidth() const noexcept;
		void setWidth(int width) noexcept;

		int getHeight() const noexcept;
		void setHeight(int height) noexcept;

	private:
		double mFov;
		double mNearPlaneDistance;
		double mFarPlaneDistance;

		Geometry::Vector2D mPos;

		int mWidth;
		int mHeight;
		
		Camera mCamera;

		PROJECTION_TYPE mProjectionType;
		Geometry::Matrix4D mProjectionMatrix;
	};
}

