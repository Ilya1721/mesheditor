module;
#include "GeometryCore/Numeric.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Plane.h"
module Camera;

import <iostream>;

import RenderSystemConsts;

using namespace Geometry;

namespace RenderSystem
{
	Camera::Camera() :
		mTarget(CAMERA_TARGET),
		mPosition(CAMERA_POSITION),
		mUp(CAMERA_UP),
		mRight(CAMERA_RIGHT)
	{
		mViewMatrix = createViewMatrix();
	}

	const Matrix4D& Camera::getViewMatrix() const
	{
		return mViewMatrix;
	}

	Matrix4D Camera::createViewMatrix() const
	{
		return Matrix4D::lookAt(mPosition, mTarget, mUp);
	}

	void Camera::setTarget(const Vector3D& newTarget)
	{
		setPositionTargetUp(mPosition, newTarget, mUp);
	}

	const Vector3D& Camera::getTarget() const
	{
		return mTarget;
	}

	void Camera::setPosition(const Vector3D& newPosition)
	{
		setPositionTargetUp(newPosition, mTarget, mUp);
	}

	const Vector3D& Camera::getPosition() const
	{
		return mPosition;
	}

	void Camera::setUp(const Vector3D& newUp)
	{
		setPositionTargetUp(mPosition, mTarget, newUp);
	}

	const Vector3D& Camera::getUp() const
	{
		return mUp;
	}

	void Camera::setPositionTargetUp(const Vector3D& newPosition, const Vector3D& newTarget, const Vector3D& newUp)
	{
		if (newUp == Vector3D(0.0f, 0.0f, 0.0f))
		{
			throw std::exception("The Camera Up vector must never be equal to null vector");
		}

		if (mTarget == newPosition)
		{
			throw std::exception("Target must never be equal to the position of the Camera");
		}

		mPosition = newPosition;
		mTarget = newTarget;
		mUp = newUp;
		mRight = calcRight();
		mViewMatrix = createViewMatrix();
	}

	void Camera::pan(const Geometry::Vector3D& firstPoint, const Geometry::Vector3D& secondPoint)
	{
		Ray firstRay(firstPoint, firstPoint - mPosition);
		Ray secondRay(secondPoint, secondPoint - mPosition);
		Plane targetPlane(mTarget, mPosition - mTarget);
		auto firstIntersectionPoint = firstRay.findIntersection(targetPlane);
		auto secondIntersectionPoint = secondRay.findIntersection(targetPlane);
		auto movement = firstIntersectionPoint - secondIntersectionPoint;

		translate(movement);
		mViewMatrix = createViewMatrix();
	}

	void Camera::translate(const Geometry::Vector3D& movement)
	{
		mPosition += movement;
		mTarget += movement;
	}

	Geometry::Vector3D Camera::calcRight()
	{
		auto direction = (mTarget - mPosition).getNormalized();
		if (!isEqual(direction * mUp, 0.0f))
		{
			throw std::exception("Camera Up and Direction vectors must be perpendicular to each other");
		}

		return mUp.cross(direction).getNormalized();
	}

	void Camera::adjust(const MeshCore::AABBox& bbox, float fov)
	{
		auto distanceToCamera = bbox.getHeight() / (2.0f * Geometry::tan(Geometry::toRadians(fov / 2.0f)));
		auto bboxCenter = bbox.getCenter();
		Geometry::Vector3D position(bboxCenter.x(), bboxCenter.y(), bbox.getMax().z() + distanceToCamera * CAMERA_DISTANCE_MULT);
		setPositionTargetUp(position, bboxCenter, mUp);
	}
}
