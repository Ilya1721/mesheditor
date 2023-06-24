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
		Camera() noexcept;

		const Geometry::Matrix4D& getViewMatrix() const noexcept;

		void setTarget(const Geometry::Vector3D& newTarget);
		const Geometry::Vector3D& getTarget() const noexcept;
		void setPosition(const Geometry::Vector3D& newPosition);
		const Geometry::Vector3D& getPosition() const noexcept;
		void setUp(const Geometry::Vector3D& newUp);
		const Geometry::Vector3D& getUp() const noexcept;
		void setPositionTargetUp(const Geometry::Vector3D& newPosition,
								 const Geometry::Vector3D& newTarget,
							     const Geometry::Vector3D& newUp);

		void translate(const Geometry::Vector3D& movement) noexcept;
		void adjust(const MeshCore::AABBox& bbox, float fov) noexcept;

	private:
		Geometry::Vector3D calcRight();
		Geometry::Matrix4D createViewMatrix() const noexcept;

	private:
		Geometry::Vector3D mTarget;
		Geometry::Vector3D mPosition;
		Geometry::Vector3D mUp;
		Geometry::Vector3D mRight;
		Geometry::Matrix4D mViewMatrix;
	};
}

