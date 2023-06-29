#pragma once

#include <glm/glm.hpp>

namespace Geometry
{
	class Matrix4D;

	using Vec3Type = glm::vec<3, float, glm::qualifier::defaultp>;
	using Vec4Type = glm::vec<4, float, glm::qualifier::defaultp>;

	class Vec4Impl
	{
	public:
		Vec4Impl() = default;
		Vec4Impl(float x, float y, float z, float w);

		Vec4Impl(const Vec4Impl& other);
		Vec4Impl(Vec4Impl&& other) noexcept;
		Vec4Impl& operator=(const Vec4Impl& other);
		Vec4Impl& operator=(Vec4Impl&& other) noexcept;
		Vec4Impl(Vec3Type&& vec) noexcept;
		Vec4Impl(Vec4Type&& vec) noexcept;

		const Vec4Type& getImplVec() const;
		const Vec3Type getVec3() const;

		bool operator==(const Vec4Impl& other) const;
		float& operator[](int index);
		float operator[](int index) const;
		Vec4Impl operator*(const Matrix4D& matrix) const;
		float operator*(const Vec4Impl& other) const;
		Vec4Impl operator+(const Vec4Impl& other) const;
		Vec4Impl operator-(const Vec4Impl& other) const;
		Vec4Impl operator/(float n) const;
		Vec4Impl operator*(float n) const;
		void operator+=(const Vec4Impl& other);
		friend Vec4Impl operator*(float n, const Vec4Impl& other);
		const float* valuePtr() const;
		Vec4Impl getNormalized() const;
		Vec4Impl cross(const Vec4Impl& other) const;
		Vec4Impl project(const Matrix4D& model, const Matrix4D& proj, const Vec4Impl& viewport) const;
		Vec4Impl unProject(const Matrix4D& model, const Matrix4D& proj, const Vec4Impl& viewport) const;

	private:
		Vec4Type mImplVec;
	};
}
