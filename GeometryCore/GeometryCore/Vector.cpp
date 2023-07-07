#include "Vector.h"

#include "VecImpl.h"

namespace Geometry
{
	Vector2D::Vector2D()
		: mpImpl(new Vec4Impl())
	{}

	Vector2D::Vector2D(float x, float y)
		: mpImpl(new Vec4Impl(x, y, 0.0f, 0.0f))
	{}

	Vector2D::Vector2D(const Vector2D& other)
		: mpImpl(new Vec4Impl(*other.mpImpl))
	{}

	Vector2D::Vector2D(Vector2D&& other) noexcept
	{
		*this = std::move(other);
	}

	Vector2D& Vector2D::operator=(const Vector2D& other)
	{
		if (this != &other)
		{
			*mpImpl = *other.mpImpl;
		}

		return *this;
	}

	Vector2D& Vector2D::operator=(Vector2D&& other) noexcept
	{
		if (this != &other)
		{
			delete mpImpl;
			mpImpl = other.mpImpl;
			other.mpImpl = nullptr;
		}

		return *this;
	}

	Vector2D::~Vector2D()
	{
		delete mpImpl;
	}

	Vector2D::Vector2D(Vec4Impl&& other) noexcept
		: mpImpl(new Vec4Impl(other))
	{}

	bool Vector2D::operator==(const Vector2D& other) const
	{
		return *mpImpl == *other.mpImpl;
	}

	float& Vector2D::operator[](int index)
	{
		return (*mpImpl)[index];
	}

	float Vector2D::operator[](int index) const
	{
		return (*mpImpl)[index];
	}

	float Vector2D::operator*(const Vector2D& other) const
	{
		return (*mpImpl) * (*other.mpImpl);
	}

	Vector2D Vector2D::operator+(const Vector2D& other) const
	{
		return (*mpImpl) + (*other.mpImpl);
	}

	void Vector2D::operator+=(const Vector2D& other)
	{
		(*mpImpl) += (*other.mpImpl);
	}

	Vector2D Vector2D::operator-(const Vector2D& other) const
	{
		return (*mpImpl) - (*other.mpImpl);
	}

	Vector2D Vector2D::operator/(float n) const
	{
		return (*mpImpl) / n;
	}

	Vector2D Vector2D::operator*(float n) const
	{
		return (*mpImpl) * n;
	}

	Vector2D operator*(float n, const Vector2D& other)
	{
		return other * n;
	}

	const float* Vector2D::valuePtr() const
	{
		return mpImpl->valuePtr();
	}

	Vector2D Vector2D::getNormalized() const
	{
		return mpImpl->getNormalized();
	}

	Vector2D Vector2D::cross(const Vector2D& other) const
	{
		return mpImpl->cross(*other.mpImpl);
	}

	Vector2D Vector2D::operator-() const
	{
		return -(*mpImpl);
	}

	float Vector2D::length() const
	{
		return mpImpl->length();
	}

	float Vector2D::x() const
	{
		return (*mpImpl)[0];
	}

	void Vector2D::setX(float x)
	{
		(*mpImpl)[0] = x;
	}

	float Vector2D::y() const
	{
		return (*mpImpl)[1];
	}

	void Vector2D::setY(float y)
	{
		(*mpImpl)[1] = y;
	}

	float Vector2D::r() const
	{
		return x();
	}

	void Vector2D::setR(float r)
	{
		setX(r);
	}

	float Vector2D::g() const
	{
		return y();
	}

	void Vector2D::setG(float g)
	{
		setY(g);
	}

	Vec4Impl* Vector2D::__internal_getPimpl() const
	{
		return mpImpl;
	}

	Vector3D::Vector3D(float x, float y, float z)
		: Vector2D(x, y)
	{
		setZ(z);
	}

	Vector3D::Vector3D(Vector2D&& other) noexcept
		: Vector2D(other)
	{}

	Vector3D::Vector3D(const Vector3D& other)
		: Vector2D(other)
	{}

	Vector3D::Vector3D(Vector3D&& other) noexcept
		: Vector2D(other)
	{}

	Vector3D::Vector3D(Vec4Impl&& other) noexcept
		: Vector2D(std::move(other))
	{}

	Vector3D& Vector3D::operator=(const Vector3D& other)
	{
		Vector2D::operator=(other);
		return *this;
	}

	Vector3D& Vector3D::operator=(Vector3D&& other) noexcept
	{
		Vector2D::operator=(other);
		return *this;
	}

	float Vector3D::operator*(const Vector3D& other) const
	{
		return Vector2D::operator*(other);
	}

	Vector3D Vector3D::operator+(const Vector3D& other) const
	{
		return Vector2D::operator+(other);
	}

	Vector3D Vector3D::operator-(const Vector3D& other) const
	{
		return Vector2D::operator-(other);
	}

	Vector3D Vector3D::operator/(float n) const
	{
		return Vector2D::operator/(n);
	}

	Vector3D Vector3D::operator*(float n) const
	{
		return Vector2D::operator*(n);
	}

	Vector3D operator*(float n, const Vector3D& other)
	{
		return other * n;
	}

	Vector3D Vector3D::getNormalized() const
	{
		return Vector2D::getNormalized();
	}

	Vector3D Vector3D::cross(const Vector3D& other) const
	{
		return Vector2D::cross(other);
	}

	Vector3D Vector3D::project(const Matrix4D& model, const Matrix4D& proj, const Vector4D& viewport) const
	{
		return mpImpl->project(model, proj, *viewport.mpImpl);
	}

	Vector3D Vector3D::unProject(const Matrix4D& model, const Matrix4D& proj, const Vector4D& viewport) const
	{
		return mpImpl->unProject(model, proj, *viewport.mpImpl);
	}

	Vector3D Vector3D::operator-() const
	{
		return Vector2D::operator-();
	}

	float Vector3D::length() const
	{
		return Vector2D::length();
	}

	float Vector3D::z() const
	{
		return (*mpImpl)[2];
	}

	void Vector3D::setZ(float z)
	{
		(*mpImpl)[2] = z;
	}

	float Vector3D::b() const
	{
		return z();
	}

	void Vector3D::setB(float b)
	{
		setZ(b);
	}

	Vector4D::Vector4D(float x, float y, float z, float w)
		: Vector3D(x, y, z)
	{
		setW(w);
	}

	Vector4D::Vector4D(const Vector3D& vec3, float w)
		: Vector3D(vec3)
	{
		setW(w);
	}

	Vector4D::Vector4D(Vector3D&& other) noexcept
		: Vector3D(other)
	{}

	Vector4D::Vector4D(Vec4Impl&& other) noexcept
		: Vector3D(std::move(other))
	{}

	Vector4D::Vector4D(const Vector4D& other)
		: Vector3D(other)
	{}

	Vector4D::Vector4D(Vector4D&& other) noexcept
		: Vector3D(other)
	{}

	Vector4D& Vector4D::operator=(const Vector4D& other)
	{
		Vector3D::operator=(other);
		return *this;
	}

	Vector4D& Vector4D::operator=(Vector4D&& other) noexcept
	{
		Vector3D::operator=(other);
		return *this;
	}

	float Vector4D::operator*(const Vector4D& other) const
	{
		return Vector3D::operator*(other);
	}

	Vector4D Vector4D::operator+(const Vector4D& other) const
	{
		return Vector3D::operator+(other);
	}

	Vector4D Vector4D::operator-(const Vector4D& other) const
	{
		return Vector3D::operator-(other);
	}

	Vector4D Vector4D::operator/(float n) const
	{
		return Vector3D::operator/(n);
	}

	Vector4D Vector4D::operator*(float n) const
	{
		return Vector3D::operator*(n);
	}

	Vector4D operator*(float n, const Vector4D& other)
	{
		return other * n;
	}

	Vector4D Vector4D::getNormalized() const
	{
		return Vector3D::getNormalized();
	}

	Vector4D Vector4D::cross(const Vector4D& other) const
	{
		return Vector3D::cross(other);
	}

	Vector4D Vector4D::operator-() const
	{
		return Vector3D::operator-();
	}

	Vector3D Vector4D::getVec3() const
	{
		return Vector3D(x(), y(), z());
	}

	float Vector4D::length() const
	{
		return Vector3D::length();
	}

	float Vector4D::w() const
	{
		return (*mpImpl)[3];
	}

	void Vector4D::setW(float w)
	{
		(*mpImpl)[3] = w;
	}

	float Vector4D::a() const
	{
		return w();
	}

	void Vector4D::setA(float a)
	{
		setW(a);
	}
}
