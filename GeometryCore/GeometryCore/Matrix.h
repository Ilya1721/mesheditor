#pragma once

namespace Geometry
{
	class Vector3D;
	class Mat4Impl;

	class Matrix4D
	{
	public:
		Matrix4D() noexcept;
		Matrix4D(float arg) noexcept;

		Matrix4D(const Matrix4D& other) noexcept;
		Matrix4D(Matrix4D&& other) noexcept;
		Matrix4D& operator=(const Matrix4D& other) noexcept;
		Matrix4D& operator=(Matrix4D&& other) noexcept;
		~Matrix4D() noexcept;

		bool operator==(const Matrix4D& other) const noexcept;
		Matrix4D operator*(const Matrix4D& other) const noexcept;
		const float* valuePtr() const noexcept;

		Mat4Impl* __internal_getPimpl() const noexcept;

		static Matrix4D lookAt(const Vector3D& mPosition, const Vector3D& mTarget, const Vector3D& mUp) noexcept;
		static Matrix4D ortho(float left, float right, float bottom, float top, float zNear, float zFar) noexcept;
		static Matrix4D perspective(float fov, float aspect, float zNear, float zFar) noexcept;

	private:
		Matrix4D(Mat4Impl&& Mat4Impl) noexcept;

	private:
		Mat4Impl* mpImpl;
	};
}

