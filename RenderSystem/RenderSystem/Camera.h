#pragma once

#include "GeometryCore/Plane.h"
#include "MeshCore/AABBox.h"

namespace RenderSystem
{
	class Camera
	{
	public:
		Camera();

		const Geometry::Matrix4D& getViewMatrix() const;

		const Geometry::Vector3D& getTarget() const;
		const Geometry::Vector3D& getPosition() const;
		const Geometry::Vector3D& getUp() const;
		const Geometry::Vector3D& getRight() const;
		Geometry::Vector3D getNormalizedDirection() const;

		void setPositionTargetUp(const Geometry::Vector3D& newPosition,
								 const Geometry::Vector3D& newTarget,
								 const Geometry::Vector3D& newUp);

		void pan(const Geometry::Vector3D& firstPoint, const Geometry::Vector3D& secondPoint);
		void zoomToPoint(const Geometry::Vector3D& unProjectedMousePos, int scrollSign);
		void adjust(const MeshCore::AABBox& bbox, float fov);

	private:
		Geometry::Vector3D calcRight();
		Geometry::Matrix4D createViewMatrix() const;
		void translate(const Geometry::Vector3D& movement);
		Geometry::Plane getTargetPlane() const;

	private:
		Geometry::Vector3D mTarget;
		Geometry::Vector3D mPos;
		Geometry::Vector3D mUp;
		Geometry::Vector3D mRight;
		Geometry::Matrix4D mViewMatrix;
	};
}

