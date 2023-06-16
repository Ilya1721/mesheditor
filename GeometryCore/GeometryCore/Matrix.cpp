#include "Matrix.h"

#include "MatImpl.h"
#include "Vector.h"

namespace Geometry
{
	Matrix4D::Matrix4D() noexcept
		: mpImpl(new Mat4Impl())
	{}

	Matrix4D::Matrix4D(double arg) noexcept
		: mpImpl(new Mat4Impl(arg))
	{}

	Matrix4D::Matrix4D(const Matrix4D& other) noexcept
		: mpImpl(new Mat4Impl(*other.mpImpl))
	{}

	Matrix4D::Matrix4D(Matrix4D&& other) noexcept
	{
		*this = std::move(other);
	}

	Matrix4D& Matrix4D::operator=(const Matrix4D& other) noexcept
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

	Matrix4D::~Matrix4D() noexcept
	{
		delete mpImpl;
	}

	Matrix4D::Matrix4D(Mat4Impl&& mat4Impl) noexcept
		: mpImpl(new Mat4Impl(mat4Impl))
	{}

	bool Matrix4D::operator==(const Matrix4D& other) const noexcept
	{
		return *mpImpl == *other.mpImpl;
	}

	Matrix4D Matrix4D::operator*(const Matrix4D& other) const noexcept
	{
		return (*mpImpl) * (*other.mpImpl);
	}

	double* Matrix4D::valuePtr() noexcept
	{
		return mpImpl->valuePtr();
	}

	Mat4Impl* Matrix4D::__internal_getPimpl() const noexcept
	{
		return mpImpl;
	}

	Matrix4D Matrix4D::lookAt(const Vector3D& mPosition, const Vector3D& mTarget, const Vector3D& mUp) noexcept
	{
		return Mat4Impl::lookAt(mPosition, mTarget, mUp);
	}

	Matrix4D Matrix4D::ortho(double left, double right, double bottom, double top, double zNear, double zFar) noexcept
	{
		return Mat4Impl::ortho(left, right, bottom, top, zNear, zFar);
	}

	Matrix4D Matrix4D::perspective(double fov, double aspect, double zNear, double zFar) noexcept
	{
		return Mat4Impl::perspective(fov, aspect, zNear, zFar);
	}
}
