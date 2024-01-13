#pragma once

#include "GeometryCore/Plane.h"
#include "MeshCore/AABBox.h"

namespace RenderSystem
{
	class Camera
	{
	public:
		Camera();

		const glm::mat4& getViewMatrix() const;

		const glm::vec3& getTarget() const;
		const glm::vec3& getEye() const;
		const glm::vec3& getUp() const;
		const glm::vec3& getRight() const;
		glm::vec3 getNormalizedDirection() const;

		void setEyeTargetUp(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);
		void pan(const glm::vec3& startPointInWorldSpace, const glm::vec3& endPointInWorldSpace);
		void orbit(const glm::vec3& startPointInNDC, const glm::vec3& endPointInNDC);
		void zoomToPoint(const glm::vec3& cursorPosInWorldSpace, float step);
		void adjust(const MeshCore::AABBox& bbox, float fov);

	private:
		glm::vec3 calcRight() const;
		glm::mat4 createViewMatrix() const;
		void translate(const glm::vec3& movement);
		glm::mat4 calculateViewMatrixWithTargetAtOrigin() const;
		GeometryCore::Plane getTargetPlane() const;
		glm::vec3 getCursorPosInNDCWithZ(const glm::vec3& cursorPosInNDC) const;
		glm::mat4 getOrbitTransform(const glm::vec3& startPosInNDCWithZ, const glm::vec3& endPosInNDCWithZ) const;

	private:
		glm::vec3 mTarget;
		glm::vec3 mEye;
		glm::vec3 mUp;
		glm::vec3 mRight;
		glm::mat4 mViewMatrix;
	};
}

