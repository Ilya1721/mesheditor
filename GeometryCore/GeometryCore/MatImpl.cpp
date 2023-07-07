#include "MatImpl.h"

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "Vector.h"
#include "VecImpl.h"
#include "Constants.h"

namespace Geometry
{
	Mat4Impl::Mat4Impl(float arg)
		: mImplMat(arg)
	{}

	Mat4Impl::Mat4Impl(const Mat4Impl& other)
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

	Mat4Impl& Mat4Impl::operator=(const Mat4Impl& other)
	{
		if (this != &other)
		{
			mImplMat = other.mImplMat;
		}

		return *this;
	}

	Mat4Impl::Mat4Impl(const MatType& matrix)
		: mImplMat(matrix)
	{}

	Mat4Impl::Mat4Impl(MatType&& matrix) noexcept
		: mImplMat(std::move(matrix))
	{}

	float* Mat4Impl::valuePtr()
	{
		return glm::value_ptr(mImplMat);
	}

	const MatType& Mat4Impl::getImplMat() const
	{
		return mImplMat;
	}

	bool Mat4Impl::operator==(const Mat4Impl& other) const
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

	Mat4Impl Mat4Impl::operator*(const Mat4Impl& other) const
	{
		return mImplMat * other.mImplMat;
	}

	Vec4Impl Mat4Impl::operator*(const Vec4Impl& other) const
	{
		return mImplMat * other.getImplVec();
	}

	Mat4Impl Mat4Impl::lookAt(const Vector3D& mPosition, const Vector3D& mTarget, const Vector3D& mUp)
	{
		return glm::lookAt(mPosition.__internal_getPimpl()->getVec3(),
						   mTarget.__internal_getPimpl()->getVec3(),
						   mUp.__internal_getPimpl()->getVec3());
	}

	Mat4Impl Mat4Impl::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		return glm::ortho(left, right, bottom, top, zNear, zFar);
	}

	Mat4Impl Mat4Impl::perspective(float fov, float aspect, float zNear, float zFar)
	{
		return glm::perspective(fov, aspect, zNear, zFar);
	}

	Mat4Impl Mat4Impl::getScaleMatrix(const Vector3D& scale)
	{
		return glm::scale(glm::mat4(1.0f), glm::vec3(scale.__internal_getPimpl()->getImplVec()));
	}

	Mat4Impl Mat4Impl::getRotationMatrix(float angle, const Vector3D& rotationAxis)
	{
		return glm::rotate(glm::mat4(1.0f), angle, glm::vec3(rotationAxis.__internal_getPimpl()->getImplVec()));
	}
}