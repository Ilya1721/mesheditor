#include "VecImpl.h"

#include <glm/gtc/epsilon.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "Matrix.h"
#include "MatImpl.h"

namespace Geometry
{
	Vec4Impl::Vec4Impl(float x, float y, float z, float w)
		: mImplVec(x, y, z, w)
	{}

	Vec4Impl::Vec4Impl(const Vec4Impl& other)
		: mImplVec(other.mImplVec)
	{}

	Vec4Impl::Vec4Impl(Vec4Impl&& other) noexcept
	{
		*this = std::move(other);
	}

	Vec4Impl& Vec4Impl::operator=(Vec4Impl&& other) noexcept
	{
		if (this != &other)
		{
			mImplVec = std::move(other.mImplVec);
		}

		return *this;
	}

	Vec4Impl& Vec4Impl::operator=(const Vec4Impl& other)
	{
		if (this != &other)
		{
			mImplVec = other.mImplVec;
		}

		return *this;
	}

	Vec4Impl::Vec4Impl(Vec3Type&& vec) noexcept
		: mImplVec(vec, 0.0f)
	{}

	Vec4Impl::Vec4Impl(Vec4Type&& vec) noexcept
		: mImplVec(vec)
	{}

	Vec4Type Vec4Impl::getImplVec() const
	{
		return mImplVec;
	}

	Vec3Type Vec4Impl::getVec3() const
	{
		return Vec3Type(mImplVec);
	}

	bool Vec4Impl::operator==(const Vec4Impl& other) const
	{
		return glm::all(glm::epsilonEqual(mImplVec, other.mImplVec, EPSILON));
	}

	float& Vec4Impl::operator[](int index)
	{
		return mImplVec[index];
	}

	float Vec4Impl::operator[](int index) const
	{
		return mImplVec[index];
	}

	float Vec4Impl::operator*(const Vec4Impl& other) const
	{
		return glm::dot(mImplVec, other.mImplVec);
	}

	Vec4Impl Vec4Impl::operator+(const Vec4Impl& other) const
	{
		return mImplVec + other.mImplVec;
	}

	void Vec4Impl::operator+=(const Vec4Impl& other)
	{
		mImplVec += other.mImplVec;
	}

	Vec4Impl operator*(float n, const Vec4Impl& other)
	{
		return n * other.mImplVec;
	}

	Vec4Impl Vec4Impl::operator-(const Vec4Impl& other) const
	{
		return mImplVec - other.mImplVec;
	}

	Vec4Impl Vec4Impl::operator/(float n) const
	{
		return mImplVec / n;
	}

	Vec4Impl Vec4Impl::operator*(float n) const
	{
		return n * (*this);
	}

	const float* Vec4Impl::valuePtr() const
	{
		return glm::value_ptr(mImplVec);
	}

	Vec4Impl Vec4Impl::getNormalized() const
	{
		return glm::normalize(mImplVec);
	}

	Vec4Impl Vec4Impl::cross(const Vec4Impl& other) const
	{
		glm::vec3 firstVec3(mImplVec);
		glm::vec3 secondVec3(other.mImplVec);
		return glm::cross(firstVec3, secondVec3);
	}

	Vec4Impl Vec4Impl::project(const Matrix4D& model, const Matrix4D& proj, const Vec4Impl& viewport) const
	{
		return glm::project(getVec3(),
			model.__internal_getPimpl()->getImplMat(),
			proj.__internal_getPimpl()->getImplMat(),
			viewport.getImplVec());
	}

	Vec4Impl Vec4Impl::unProject(const Matrix4D& model, const Matrix4D& proj, const Vec4Impl& viewport) const
	{
		return glm::unProject(getVec3(),
			model.__internal_getPimpl()->getImplMat(),
			proj.__internal_getPimpl()->getImplMat(),
			viewport.getImplVec());
	}

	Vec4Impl Vec4Impl::operator-() const
	{
		return -mImplVec;
	}

	float Vec4Impl::length() const
	{
		return glm::length(mImplVec);
	}
}
