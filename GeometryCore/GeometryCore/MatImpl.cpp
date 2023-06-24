#include "MatImpl.h"

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vector.h"
#include "VecImpl.h"
#include "Constants.h"

namespace Geometry
{
	Mat4Impl::Mat4Impl(float arg) noexcept
		: mImplMat(arg)
	{}

	Mat4Impl::Mat4Impl(const Mat4Impl& other) noexcept
		: mImplMat(other.mImplMat)
	{}

	Mat4Impl::Mat4Impl(Mat4Impl&& other) noexcept
	{
		*this = std::move(other);
	}

	Mat4Impl& Mat4Impl::operator=(Mat4Impl&& other) noexcept
	{
		if (this != &other)
		{
			mImplMat = std::move(other.mImplMat);
		}

		return *this;
	}

	Mat4Impl& Mat4Impl::operator=(const Mat4Impl& other) noexcept
	{
		if (this != &other)
		{
			mImplMat = other.mImplMat;
		}

		return *this;
	}

	Mat4Impl::Mat4Impl(const MatType& matrix) noexcept
		: mImplMat(matrix)
	{}

	Mat4Impl::Mat4Impl(MatType&& matrix) noexcept
		: mImplMat(std::move(matrix))
	{}

	float* Mat4Impl::valuePtr() noexcept
	{
		return glm::value_ptr(mImplMat);
	}

	const MatType& Mat4Impl::getImplMat() const noexcept
	{
		return mImplMat;
	}

	bool Mat4Impl::operator==(const Mat4Impl& other) const noexcept
	{
		if (mImplMat.length() != other.mImplMat.length())
			return false;

		for (int matIdx = 0; matIdx < mImplMat.length(); ++matIdx)
		{
			if (!glm::all(glm::epsilonEqual(mImplMat[matIdx], other.mImplMat[matIdx], EPSILON)))
			{
				return false;
			}
		}

		return true;
	}

	Mat4Impl Mat4Impl::operator*(const Mat4Impl& other) const noexcept
	{
		return mImplMat * other.mImplMat;
	}

	Mat4Impl Mat4Impl::lookAt(const Vector3D& mPosition, const Vector3D& mTarget, const Vector3D& mUp) noexcept
	{
		return glm::lookAt(mPosition.__internal_getPimpl()->getVec3(),
						   mTarget.__internal_getPimpl()->getVec3(),
						   mUp.__internal_getPimpl()->getVec3());
	}

	Mat4Impl Mat4Impl::ortho(float left, float right, float bottom, float top, float zNear, float zFar) noexcept
	{
		return glm::ortho(left, right, bottom, top, zNear, zFar);
	}

	Mat4Impl Mat4Impl::perspective(float fov, float aspect, float zNear, float zFar) noexcept
	{
		return glm::perspective(fov, aspect, zNear, zFar);
	}
}
