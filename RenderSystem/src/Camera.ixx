module;
#include "GeometryCore/Vector.h"
#include "GeometryCore/Matrix.h"
export module Camera;

export namespace RenderSystem
{
	class Camera
	{
	public:
		Camera() noexcept;

		Geometry::Matrix4D getViewMatrix() const noexcept;
		void calcViewMatrix() noexcept;

		void setTarget(const Geometry::Vector3D& newTarget) noexcept;
		const Geometry::Vector3D& getTarget() const noexcept;

		void setPosition(const Geometry::Vector3D& newPosition) noexcept;
		const Geometry::Vector3D& getPosition() const noexcept;

		void setUp(const Geometry::Vector3D& newUp) noexcept;
		const Geometry::Vector3D& getUp() const noexcept;

		void setPositionTargetUp(const Geometry::Vector3D& newPosition,
								 const Geometry::Vector3D& newTarget,
							     const Geometry::Vector3D& newUp) noexcept;

		void translate(const Geometry::Vector3D& movement) noexcept;

	private:
		void recalcCamera() noexcept;

	private:
		Geometry::Vector3D mTarget;
		Geometry::Vector3D mPosition;
		Geometry::Vector3D mDirection;
		Geometry::Vector3D mUp;
		Geometry::Vector3D mRight;
		Geometry::Matrix4D mViewMatrix;
	};
}

