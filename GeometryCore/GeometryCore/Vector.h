#pragma once

namespace Geometry
{
	class Matrix4D;
	class Vec4Impl;

	class Vector2D
	{
	public:
		Vector2D() noexcept;
		Vector2D(float x, float y) noexcept;

		Vector2D(const Vector2D& other) noexcept;
		Vector2D(Vector2D&& other) noexcept;
		Vector2D& operator=(const Vector2D& other) noexcept;
		Vector2D& operator=(Vector2D&& other) noexcept;
		~Vector2D() noexcept;

		bool operator==(const Vector2D& other) const noexcept;
		float& operator[](int index) noexcept;
		float operator[](int index) const noexcept;
		Vector2D operator+(const Vector2D& other) const noexcept;
		Vector2D operator-(const Vector2D& other) const noexcept;
		Vector2D operator/(float n) const noexcept;
		void operator+=(const Vector2D& other) noexcept;

		float x() const noexcept;
		void setX(float x) noexcept;
		float y() const noexcept;
		void setY(float y) noexcept;

		float r() const noexcept;
		void setR(float r) noexcept;
		float g() const noexcept;
		void setG(float g) noexcept;

		Vec4Impl* __internal_getPimpl() const noexcept;

		static Vector2D normalize(const Vector2D& vec) noexcept;
		static float dot(const Vector2D& firstVec, const Vector2D& secondVec) noexcept;
		static Vector2D cross(const Vector2D& firstVec, const Vector2D& secondVec) noexcept;
	protected:
		Vector2D(Vec4Impl&& other) noexcept;

	protected:
		Vec4Impl* mpImpl;
	};

	class Vector4D;

	class Vector3D : public Vector2D
	{
	public:
		Vector3D() noexcept = default;
		Vector3D(float x, float y, float z) noexcept;
		Vector3D(Vector2D&& other) noexcept;

		Vector3D(const Vector3D& other) noexcept;
		Vector3D(Vector3D&& other) noexcept;
		Vector3D& operator=(const Vector3D& other) noexcept;
		Vector3D& operator=(Vector3D&& other) noexcept;

		Vector3D operator+(const Vector3D& other) const noexcept;
		Vector3D operator-(const Vector3D& other) const noexcept;
		Vector3D operator/(float n) const noexcept;

		float z() const noexcept;
		void setZ(float z) noexcept;
		float b() const noexcept;
		void setB(float b) noexcept;

		static Vector3D normalize(const Vector3D& vec) noexcept;
		static float dot(const Vector3D& firstVec, const Vector3D& secondVec) noexcept;
		static Vector3D cross(const Vector3D& firstVec, const Vector3D& secondVec) noexcept;
		static Vector3D project(const Vector3D& obj, const Matrix4D& model, const Matrix4D& proj, const Vector4D& viewport) noexcept;
		static Vector3D unProject(const Vector3D& win, const Matrix4D& model, const Matrix4D& proj, const Vector4D& viewport) noexcept;

	protected:
		Vector3D(Vec4Impl&& other) noexcept;
	};

	class Vector4D : public Vector3D
	{
	public:
		Vector4D() noexcept = default;
		Vector4D(float x, float y, float z, float w) noexcept;
		Vector4D(Vector3D&& other) noexcept;

		Vector4D(const Vector4D& other) noexcept;
		Vector4D(Vector4D&& other) noexcept;
		Vector4D& operator=(const Vector4D& other) noexcept;
		Vector4D& operator=(Vector4D&& other) noexcept;

		Vector4D operator+(const Vector4D& other) const noexcept;
		Vector4D operator-(const Vector4D& other) const noexcept;
		Vector4D operator/(float n) const noexcept;
		Vector4D operator*(const Matrix4D& matrix) const noexcept;

		Vector3D getVec3() const noexcept;

		float w() const noexcept;
		void setW(float w) noexcept;
		float a() const noexcept;
		void setA(float b) noexcept;

		static Vector4D normalize(const Vector4D& vec) noexcept;
		static float dot(const Vector4D& firstVec, const Vector4D& secondVec) noexcept;
		static Vector4D cross(const Vector4D& firstVec, const Vector4D& secondVec) noexcept;

	protected:
		Vector4D(Vec4Impl&& other) noexcept;
	};
}
