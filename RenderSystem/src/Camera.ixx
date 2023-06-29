module;
#include "GeometryCore/Vector.h"
#include "GeometryCore/Matrix.h"
export module Camera;

import AABBox;

export namespace RenderSystem
{
	class Camera
	{
	public:
		Camera();

		const Geometry::Matrix4D& getViewMatrix() const;

		void setTarget(const Geometry::Vector3D& newTarget);
		const Geometry::Vector3D& getTarget() const;
		void setPosition(const Geometry::Vector3D& newPosition);
		const Geometry::Vector3D& getPosition() const;
		void setUp(const Geometry::Vector3D& newUp);
		const Geometry::Vector3D& getUp() const;
		void setPositionTargetUp(const Geometry::Vector3D& newPosition,
								 const Geometry::Vector3D& newTarget,
							     const Geometry::Vector3D& newUp);

		void pan(const Geometry::Vector3D& firstPoint, const Geometry::Vector3D& secondPoint);
		void adjust(const MeshCore::AABBox& bbox, float fov);

	private:
		Geometry::Vector3D calcRight();
		Geometry::Matrix4D createViewMatrix() const;
		void translate(const Geometry::Vector3D& movement);

	private:
		Geometry::Vector3D mTarget;
		Geometry::Vector3D mPosition;
		Geometry::Vector3D mUp;
		Geometry::Vector3D mRight;
		Geometry::Matrix4D mViewMatrix;
	};
}

