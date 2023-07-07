#include "Matrix.h"

#include "MatImpl.h"
#include "VecImpl.h"
#include "Vector.h"

namespace Geometry
{
	Matrix4D::Matrix4D()
		: mpImpl(new Mat4Impl())
	{}

	Matrix4D::Matrix4D(float arg)
		: mpImpl(new Mat4Impl(arg))
	{}

	Matrix4D::Matrix4D(const Matrix4D& other)
		: mpImpl(new Mat4Impl(*other.mpImpl))
	{}

	Matrix4D::Matrix4D(Matrix4D&& other) noexcept
	{
		*this = std::move(other);
	}

	Matrix4D& Matrix4D::operator=(const Matrix4D& other)
	{
		if (this != &other)
		{
			*mpImpl = *other.mpImpl;
		}

		return *this;
	}

	Matrix4D& Matrix4D::operator=(Matrix4D&& other) noexcept
	{
		if (this != &other)
		{
			delete mpImpl;
			mpImpl = other.mpImpl;
			other.mpImpl = nullptr;
		}

		return *this;
	}

	Matrix4D::~Matrix4D()
	{
		delete mpImpl;
	}

	Matrix4D::Matrix4D(Mat4Impl&& mat4Impl) noexcept
		: mpImpl(new Mat4Impl(mat4Impl))
	{}

	bool Matrix4D::operator==(const Matrix4D& other) const
	{
		return *mpImpl == *other.mpImpl;
	}

	Matrix4D Matrix4D::operator*(const Matrix4D& other) const
	{
		return (*mpImpl) * (*other.mpImpl);
	}

	Vector4D Matrix4D::operator*(const Vector4D& other) const
	{
		return (*mpImpl) * (*other.__internal_getPimpl());
	}

	const float* Matrix4D::valuePtr() const
	{
		return mpImpl->valuePtr();
	}

	Mat4Impl* Matrix4D::__internal_getPimpl() const
	{
		return mpImpl;
	}

	Matrix4D Matrix4D::lookAt(const Vector3D& mPosition, const Vector3D& mTarget, const Vector3D& mUp)
	{
		return Mat4Impl::lookAt(mPosition, mTarget, mUp);
	}

	Matrix4D Matrix4D::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		return Mat4Impl::ortho(left, right, bottom, top, zNear, zFar);
	}

	Matrix4D Matrix4D::perspective(float fov, float aspect, float zNear, float zFar)
	{
		return Mat4Impl::perspective(fov, aspect, zNear, zFar);
	}

	Matrix4D Matrix4D::getScaleMatrix(const Vector3D& scale)
	{
		return Mat4Impl::getScaleMatrix(scale);
	}

	Matrix4D Matrix4D::getRotationMatrix(float angle, const Vector3D& rotationAxis)
	{
		return Mat4Impl::getRotationMatrix(angle, rotationAxis);
	}
}
