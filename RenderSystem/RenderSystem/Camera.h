#pragma once

#include <functional>
#include <vector>

#include "GeometryCore/Typedefs.h"
#include "Utility/CallbackMechanism.h"

#include "Viewport.h"

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
		static Camera& getInstance();

		const glm::mat4& getViewMatrix() const;
		const Point3D& getTarget() const;
		const Point3D& getEye() const;
		const Vector3D& getUp() const;
		const Vector3D& getRight() const;
		Vector3D getNormalizedDirection() const;
		Point3D projectToTargetPlane(const Point3D& cursorPosInWorldSpace) const;
		bool isMovementEnabled() const;

		void pan(const Point3D& startPointInWorldSpace, const Point3D& endPointInWorldSpace, PROJECTION_TYPE projectionType);
		void rawOrbit(const Point3D& startPointInNDC, const Point3D& endPointInNDC);
		void smoothOrbit(float xOffset, float yOffset);
		void zoom(float step);
		void enableMovement(bool isEnabled);
		void addOnCameraEditedCallback(const std::function<void()>& callback);

	private:
		Camera();

		void init();
		void invokeEditOperation(const std::function<void()>& action);
		void setEyeTargetUp(const Point3D& eye, const Point3D& target, const Vector3D& up);
		void translate(const Vector3D& movement);
		void perspectiveAdjust(const MeshCore::AABBox& bbox, float fov);
		void orthoAdjust(const MeshCore::AABBox& bbox);
		void adjust();
		void adjust(const MeshCore::AABBox& bbox, float fov = 0.0f);
		void rotateCamera(const glm::mat4& rotationTransform);

		Vector3D calcRight() const;
		glm::mat4 createViewMatrix() const;
		glm::mat4 calculateViewMatrixWithTargetAtOrigin() const;
		Plane getTargetPlane() const;
		Point3D getCursorPosInNDCWithZ(const Point3D& cursorPosInNDC) const;
		glm::mat4 getOrbitTransform(const Point3D& startPosInNDCWithZ, const Point3D& endPosInNDCWithZ) const;
		glm::mat4 getSmoothOrbitTransform(float yaw, float pitch) const;
		glm::mat4 getCorrectionOrbitTransform() const;
		void validateCamera() const;
		float calculateDistanceToCamera(const MeshCore::AABBox& bbox, float fov = 0.0f) const;
		glm::vec3 getPanTranslationVector(const Point3D& startPointInWorldSpace, const Point3D& endPointInWorldSpace, PROJECTION_TYPE projectionType) const;

	private:
		Point3D mTarget;
		Point3D mEye;
		Vector3D mUp;
		Vector3D mRight;
		glm::mat4 mViewMatrix;
		bool mMovementEnabled;
		Utility::CallbackMechanism mCameraEditedCM;
	};
}

