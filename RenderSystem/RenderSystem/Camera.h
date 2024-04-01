#pragma once

#include <glm/glm.hpp>

#include "GeometryCore/Typedefs.h"

namespace GeometryCore
{
	struct Ray;
	struct Plane;
}

namespace MeshCore
{
	class AABBox;
}

namespace RenderSystem
{
	using namespace GeometryCore;

	class Camera
	{
	public:
		Camera();

		const glm::mat4& getViewMatrix() const;
		const Point3D& getTarget() const;
		const Point3D& getEye() const;
		const Vector3D& getUp() const;
		const Vector3D& getRight() const;
		Vector3D getNormalizedDirection() const;
		Point3D projectToTargetPlane(const Point3D& cursorPosInWorldSpace) const;
		Ray getCameraRay(const Point3D& cursorPosInWorldSpace) const;

		void setEyeTargetUp(const Point3D& eye, const Point3D& target, const Vector3D& up);
		void pan(const Point3D& startPointInWorldSpace, const Point3D& endPointInWorldSpace);
		void orbit(const Point3D& startPointInNDC, const Point3D& endPointInNDC);
		void zoom(float step);
		void adjust(const MeshCore::AABBox& bbox, float fov);

	private:
		Vector3D calcRight() const;
		glm::mat4 createViewMatrix() const;
		void translate(const Vector3D& movement);
		glm::mat4 calculateViewMatrixWithTargetAtOrigin() const;
		Plane getTargetPlane() const;
		Point3D getCursorPosInNDCWithZ(const Point3D& cursorPosInNDC) const;
		glm::mat4 getOrbitTransform(const Point3D& startPosInNDCWithZ, const Point3D& endPosInNDCWithZ) const;

	private:
		Point3D mTarget;
		Point3D mEye;
		Vector3D mUp;
		Vector3D mRight;
		glm::mat4 mViewMatrix;
	};
}

