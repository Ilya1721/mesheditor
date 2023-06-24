#include "VecImpl.h"

#include <glm/gtc/epsilon.hpp>
#include <glm/ext/matrix_projection.hpp>

#include "Constants.h"
#include "Matrix.h"
#include "MatImpl.h"

namespace Geometry
{
	Vec4Impl::Vec4Impl(float x, float y, float z, float w) noexcept
		: mImplVec(x, y, z, w)
	{}

	Vec4Impl::Vec4Impl(const Vec4Impl& other) noexcept
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

	Vec4Impl& Vec4Impl::operator=(const Vec4Impl& other) noexcept
	{
		if (this != &other)
		{
			mImplVec = other.mImplVec;
		}

		return *this;
	}

	Vec4Impl::Vec4Impl(Vec3Type&& vec) noexcept
		: mImplVec(vec, 1.0f)
	{}

	Vec4Impl::Vec4Impl(Vec4Type&& vec) noexcept
		: mImplVec(vec)
	{}

	const Vec4Type& Vec4Impl::getImplVec() const noexcept
	{
		return mImplVec;
	}

	const Vec3Type Vec4Impl::getVec3() const noexcept
	{
		return Vec3Type(mImplVec);
	}

	bool Vec4Impl::operator==(const Vec4Impl& other) const noexcept
	{
		return glm::all(glm::epsilonEqual(mImplVec, other.mImplVec, EPSILON));
	}

	float& Vec4Impl::operator[](int index) noexcept
	{
		return mImplVec[index];
	}

	float Vec4Impl::operator[](int index) const noexcept
	{
		return mImplVec[index];
	}

	Vec4Impl Vec4Impl::operator*(const Matrix4D& matrix) const noexcept
	{
		return (mImplVec * matrix.__internal_getPimpl()->getImplMat());
	}

	Vec4Impl Vec4Impl::operator+(const Vec4Impl& other) const noexcept
	{
		return mImplVec + other.mImplVec;
	}

	void Vec4Impl::operator+=(const Vec4Impl& other) noexcept
	{
		mImplVec += other.mImplVec;
	}

	Vec4Impl Vec4Impl::operator-(const Vec4Impl& other) const noexcept
	{
		return mImplVec - other.mImplVec;
	}

	Vec4Impl Vec4Impl::operator/(float n) const noexcept
	{
		return mImplVec / n;
	}

	Vec4Impl Vec4Impl::normalize(const Vec4Impl& vec) noexcept
	{
		return glm::normalize(vec.mImplVec);
	}

	float Vec4Impl::dot(const Vec4Impl& firstVec, const Vec4Impl& secondVec) noexcept
	{
		return glm::dot(firstVec.mImplVec, secondVec.mImplVec);
	}

	Vec4Impl Vec4Impl::cross(const Vec4Impl& firstVec, const Vec4Impl& secondVec) noexcept
	{
		glm::vec3 firstVec3(firstVec.mImplVec);
		glm::vec3 secondVec3(secondVec.mImplVec);
		return glm::cross(firstVec3, secondVec3);
	}

	Vec4Impl Vec4Impl::project(const Vec4Impl& obj, const Matrix4D& model, const Matrix4D& proj, const Vec4Impl& viewport) noexcept
	{
		return glm::project(obj.getVec3(),
			model.__internal_getPimpl()->getImplMat(),
			proj.__internal_getPimpl()->getImplMat(),
			viewport.getImplVec());
	}

	Vec4Impl Vec4Impl::unProject(const Vec4Impl& win, const Matrix4D& model, const Matrix4D& proj, const Vec4Impl& viewport) noexcept
	{
		return glm::unProject(win.getVec3(),
			model.__internal_getPimpl()->getImplMat(),
			proj.__internal_getPimpl()->getImplMat(),
			viewport.getImplVec());
	}
}
