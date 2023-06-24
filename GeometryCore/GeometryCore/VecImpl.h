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
		Vec4Impl() noexcept = default;
		Vec4Impl(float x, float y, float z, float w) noexcept;

		Vec4Impl(const Vec4Impl& other) noexcept;
		Vec4Impl(Vec4Impl&& other) noexcept;
		Vec4Impl& operator=(const Vec4Impl& other) noexcept;
		Vec4Impl& operator=(Vec4Impl&& other) noexcept;
		Vec4Impl(Vec3Type&& vec) noexcept;
		Vec4Impl(Vec4Type&& vec) noexcept;

		const Vec4Type& getImplVec() const noexcept;
		const Vec3Type getVec3() const noexcept;

		bool operator==(const Vec4Impl& other) const noexcept;
		float& operator[](int index) noexcept;
		float operator[](int index) const noexcept;
		Vec4Impl operator*(const Matrix4D& matrix) const noexcept;
		Vec4Impl operator+(const Vec4Impl& other) const noexcept;
		Vec4Impl operator-(const Vec4Impl& other) const noexcept;
		Vec4Impl operator/(float n) const noexcept;
		void operator+=(const Vec4Impl& other) noexcept;

		static Vec4Impl normalize(const Vec4Impl& vec) noexcept;
		static float dot(const Vec4Impl& firstVec, const Vec4Impl& secondVec) noexcept;
		static Vec4Impl cross(const Vec4Impl& firstVec, const Vec4Impl& secondVec) noexcept;
		static Vec4Impl project(const Vec4Impl& obj, const Matrix4D& model, const Matrix4D& proj, const Vec4Impl& viewport) noexcept;
		static Vec4Impl unProject(const Vec4Impl& win, const Matrix4D& model, const Matrix4D& proj, const Vec4Impl& viewport) noexcept;

	private:
		Vec4Type mImplVec;
	};
}
