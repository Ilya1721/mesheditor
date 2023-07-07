module;
#include "GeometryCore/Numeric.h"
#include "GeometryCore/Ray.h"
#include "GeometryCore/Constants.h"
module Camera;

import <iostream>;

import RenderSystemConsts;

using namespace Geometry;

namespace RenderSystem
{
	Camera::Camera() :
		mTarget(CAMERA_TARGET),
		mPos(CAMERA_POSITION),
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
		return Matrix4D::lookAt(mPos, mTarget, mUp);
	}

	const Vector3D& Camera::getTarget() const
	{
		return mTarget;
	}

	const Vector3D& Camera::getPosition() const
	{
		return mPos;
	}

	const Vector3D& Camera::getUp() const
	{
		return mUp;
	}

	const Geometry::Vector3D& Camera::getRight() const
	{
		return mRight;
	}

	Geometry::Vector3D Camera::getNormalizedDirection() const
	{
		return (mTarget - mPos).getNormalized();
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

		mPos = newPosition;
		mTarget = newTarget;
		mUp = newUp;
		mRight = calcRight();
		mViewMatrix = createViewMatrix();
	}

	void Camera::pan(const Geometry::Vector3D& firstPoint, const Geometry::Vector3D& secondPoint)
	{
		Ray firstRay(firstPoint, firstPoint - mPos);
		Ray secondRay(secondPoint, secondPoint - mPos);
		auto targetPlane = getTargetPlane();
		auto firstIntersectionPoint = firstRay.findIntersection(targetPlane);
		auto secondIntersectionPoint = secondRay.findIntersection(targetPlane);
		auto movement = firstIntersectionPoint - secondIntersectionPoint;
		translate(movement);
		mViewMatrix = createViewMatrix();
	}

	void Camera::zoomToPoint(const Geometry::Vector3D& unProjectedMousePos, int scrollSign)
	{
		Ray zoomRay(mPos, unProjectedMousePos - mPos);
		auto targetPlane = getTargetPlane();
		auto intersectionPoint = zoomRay.findIntersection(targetPlane);
		mTarget = intersectionPoint;
		auto direction = mTarget - mPos;
		mPos += direction * static_cast<float>(scrollSign) * ZOOM_STEP_KOEF;

		Vector3D xozDirection(direction.x(), 0.0, direction.z());
		auto rotateRightVecAngle = PI_ON_TWO - angle(xozDirection, mRight);
		auto rotateDegrees = toDegrees(rotateRightVecAngle);
		mRight = (Matrix4D::getRotationMatrix(rotateRightVecAngle, mUp) * Vector4D(mRight, 0.0f)).getNormalized();
		mUp = direction.getNormalized().cross(mRight);

		auto rightLength = mRight.length();
		auto upLength = mUp.length();

		auto upRightAngle = toDegrees(angle(mUp, mRight));
		auto upDirRight = toDegrees(angle(direction, mRight));
		auto upDirUp = toDegrees(angle(mUp, direction));

		mViewMatrix = createViewMatrix();
	}

	void Camera::translate(const Geometry::Vector3D& movement)
	{
		mPos += movement;
		mTarget += movement;
	}

	Plane Camera::getTargetPlane() const
	{
		return { mTarget, mPos - mTarget };
	}

	Geometry::Vector3D Camera::calcRight()
	{
		auto direction = (mTarget - mPos).getNormalized();
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
		Geometry::Vector3D position(bboxCenter.x(), bboxCenter.y(), bbox.getMax().z() + distanceToCamera * CAMERA_DIST_TO_BBOX_KOEF);
		setPositionTargetUp(position, bboxCenter, mUp);
	}
}
