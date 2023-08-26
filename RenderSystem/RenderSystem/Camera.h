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
		const glm::vec3& getPosition() const;
		const glm::vec3& getUp() const;
		const glm::vec3& getRight() const;
		glm::vec3 getNormalizedDirection() const;

		void setPositionTargetUp(const glm::vec3& newPosition,
								 const glm::vec3& newTarget,
								 const glm::vec3& newUp);

		void pan(const glm::vec3& firstPoint, const glm::vec3& secondPoint);
		void orbit(const glm::vec3& firstPoint, const glm::vec3& secondPoint);
		void zoomToPoint(const glm::vec3& unProjectedMousePos, int scrollSign, float step);
		void adjust(const MeshCore::AABBox& bbox, float fov);

	private:
		glm::vec3 calcRight() const;
		glm::mat4 createViewMatrix() const;
		void translate(const glm::vec3& movement);
		GeometryCore::Plane getTargetPlane() const;
		glm::vec3 getPointOnArcball(const glm::vec3& mousePosNDC) const;

	private:
		glm::vec3 mTarget;
		glm::vec3 mPos;
		glm::vec3 mUp;
		glm::vec3 mRight;
		glm::mat4 mViewMatrix;
	};
}

