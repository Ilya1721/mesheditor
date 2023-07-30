#pragma once

namespace Geometry
{
	class Vec4Impl;

	class Vector
	{
	public:
		Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);

		Vector(Vec4Impl&& other) noexcept;
		Vector(const Vector& other);
		Vector(Vector&& other) noexcept;
		Vector& operator=(const Vector& other);
		Vector& operator=(Vector&& other) noexcept;
		virtual ~Vector() noexcept;

		bool operator==(const Vector& other) const;
		bool operator!=(const Vector& other) const;
		float& operator[](int index);
		float operator[](int index) const;
		float operator*(const Vector& other) const;
		Vector operator+(const Vector& other) const;
		Vector operator-(const Vector& other) const;
		Vector operator/(float n) const;
		void operator+=(const Vector& other);
		Vector operator*(float n) const;
		Vector operator-() const;
		friend Vector operator*(float n, const Vector& other);

		const float* valuePtr() const;
		Vector getNormalized() const;
		float length() const;
		const char* getPrettyString() const;

		float x() const;
		void setX(float x);
		float y() const;
		void setY(float y);

		float r() const;
		void setR(float r);
		float g() const;
		void setG(float g);

		float z() const;
		void setZ(float z);
		float b() const;
		void setB(float b);

		float w() const;
		void setW(float w);
		float a() const;
		void setA(float b);

		Vec4Impl* __internal_getPimpl() const;

	protected:
		Vec4Impl* mpImpl;
	};

	class Matrix4D;
	class Vector3D;

	class Vector2D : public Vector
	{
	public:
		Vector2D(float x = 0.0f, float y = 0.0f);
		Vector2D(const Vector3D& vec3);
		Vector2D(Vector3D&& vec3);
		Vector2D(Vec4Impl&& other) noexcept;
		Vector2D(const Vector& other) noexcept;
		Vector2D(Vector&& other) noexcept;
	};

	class Vector4D;

	class Vector3D : public Vector
	{
	public:
		Vector3D(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		Vector3D(const Vector2D& vec2, float z = 0.0f) noexcept;
		Vector3D(const Vector4D& vec4) noexcept;
		Vector3D(Vector4D&& vec4) noexcept;
		Vector3D(Vec4Impl&& other) noexcept;
		Vector3D(const Vector& other) noexcept;
		Vector3D(Vector&& other) noexcept;

		Vector3D cross(const Vector3D& other) const;
		Vector3D project(const Matrix4D& modelView, const Matrix4D& proj, const Vector4D& viewport) const;
		Vector3D unProject(const Matrix4D& modelView, const Matrix4D& proj, const Vector4D& viewport) const;
	};

	class Vector4D : public Vector
	{
	public:
		Vector4D(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
		Vector4D(const Vector2D& vec2, float z = 0.0f, float w = 0.0f);
		Vector4D(const Vector3D& vec3, float w = 0.0f);
		Vector4D(Vec4Impl&& other) noexcept;
		Vector4D(const Vector& other) noexcept;
		Vector4D(Vector&& other) noexcept;

		Vector3D getVec3() const;
	};
}
