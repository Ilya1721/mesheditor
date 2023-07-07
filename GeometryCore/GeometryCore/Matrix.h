#pragma once

namespace Geometry
{
	class Vector3D;
	class Vector4D;
	class Mat4Impl;

	class Matrix4D
	{
	public:
		Matrix4D();
		Matrix4D(float arg);

		Matrix4D(const Matrix4D& other);
		Matrix4D(Matrix4D&& other) noexcept;
		Matrix4D& operator=(const Matrix4D& other);
		Matrix4D& operator=(Matrix4D&& other) noexcept;
		Matrix4D(Mat4Impl&& Mat4Impl) noexcept;
		~Matrix4D();

		bool operator==(const Matrix4D& other) const;
		Matrix4D operator*(const Matrix4D& other) const;
		Vector4D operator*(const Vector4D& other) const;
		const float* valuePtr() const;

		Mat4Impl* __internal_getPimpl() const;

		static Matrix4D lookAt(const Vector3D& mPosition, const Vector3D& mTarget, const Vector3D& mUp);
		static Matrix4D ortho(float left, float right, float bottom, float top, float zNear, float zFar);
		static Matrix4D perspective(float fov, float aspect, float zNear, float zFar);
		static Matrix4D getScaleMatrix(const Vector3D& scale);
		static Matrix4D getRotationMatrix(float angle, const Vector3D& rotationAxis);

	private:
		Mat4Impl* mpImpl;
	};
}

