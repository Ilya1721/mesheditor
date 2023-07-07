#pragma once

namespace Geometry
{
	class Matrix4D;
	class Vec4Impl;

	class Vector2D
	{
	public:
		Vector2D();
		Vector2D(float x, float y);

		Vector2D(const Vector2D& other);
		Vector2D(Vector2D&& other) noexcept;
		Vector2D& operator=(const Vector2D& other);
		Vector2D& operator=(Vector2D&& other) noexcept;
		~Vector2D();

		bool operator==(const Vector2D& other) const;
		float& operator[](int index);
		float operator[](int index) const;
		float operator*(const Vector2D& other) const;
		Vector2D operator+(const Vector2D& other) const;
		Vector2D operator-(const Vector2D& other) const;
		Vector2D operator/(float n) const;
		void operator+=(const Vector2D& other);
		Vector2D operator*(float n) const;
		friend Vector2D operator*(float n, const Vector2D& other);
		const float* valuePtr() const;
		Vector2D getNormalized() const;
		Vector2D cross(const Vector2D& other) const;
		Vector2D operator-() const;
		float length() const;

		float x() const;
		void setX(float x);
		float y() const;
		void setY(float y);

		float r() const;
		void setR(float r);
		float g() const;
		void setG(float g);

		Vec4Impl* __internal_getPimpl() const;

	protected:
		Vector2D(Vec4Impl&& other) noexcept;

	protected:
		Vec4Impl* mpImpl;
	};

	class Vector4D;

	class Vector3D : public Vector2D
	{
	public:
		Vector3D() = default;
		Vector3D(float x, float y, float z);
		Vector3D(Vector2D&& other) noexcept;

		Vector3D(const Vector3D& other);
		Vector3D(Vector3D&& other) noexcept;
		Vector3D& operator=(const Vector3D& other);
		Vector3D& operator=(Vector3D&& other) noexcept;

		float operator*(const Vector3D& other) const;
		Vector3D operator+(const Vector3D& other) const;
		Vector3D operator-(const Vector3D& other) const;
		Vector3D operator/(float n) const;
		Vector3D operator*(float n) const;
		friend Vector3D operator*(float n, const Vector3D& other);
		Vector3D getNormalized() const;
		Vector3D cross(const Vector3D& other) const;
		Vector3D project(const Matrix4D& model, const Matrix4D& proj, const Vector4D& viewport) const;
		Vector3D unProject(const Matrix4D& model, const Matrix4D& proj, const Vector4D& viewport) const;
		Vector3D operator-() const;
		float length() const;

		float z() const;
		void setZ(float z);
		float b() const;
		void setB(float b);

	protected:
		Vector3D(Vec4Impl&& other) noexcept;
	};

	class Vector4D : public Vector3D
	{
	public:
		Vector4D() = default;
		Vector4D(float x, float y, float z, float w);
		Vector4D(const Vector3D& vec3, float w);
		Vector4D(Vector3D&& other) noexcept;
		Vector4D(Vec4Impl&& other) noexcept;

		Vector4D(const Vector4D& other);
		Vector4D(Vector4D&& other) noexcept;
		Vector4D& operator=(const Vector4D& other);
		Vector4D& operator=(Vector4D&& other) noexcept;

		float operator*(const Vector4D& other) const;
		Vector4D operator+(const Vector4D& other) const;
		Vector4D operator-(const Vector4D& other) const;
		Vector4D operator/(float n) const;
		Vector4D operator*(float n) const;
		friend Vector4D operator*(float n, const Vector4D& other);
		Vector4D getNormalized() const;
		Vector4D cross(const Vector4D& other) const;
		Vector4D operator-() const;
		Vector3D getVec3() const;
		float length() const;

		float w() const;
		void setW(float w);
		float a() const;
		void setA(float b);
	};
}
