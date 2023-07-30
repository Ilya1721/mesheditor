#include "Vector.h"

#include "VecImpl.h"

namespace Geometry
{
	Vector::Vector(float x, float y, float z, float w)
		: mpImpl(new Vec4Impl(x, y, z, w))
	{}

	Vector::Vector(const Vector& other)
		: mpImpl(new Vec4Impl(*other.mpImpl))
	{}

	Vector::Vector(Vector&& other) noexcept
		: mpImpl(nullptr)
	{
		*this = std::move(other);
	}

	Vector& Vector::operator=(const Vector& other)
	{
		if (this != &other)
		{
			*mpImpl = *other.mpImpl;
		}

		return *this;
	}

	Vector& Vector::operator=(Vector&& other) noexcept
	{
		if (this != &other)
		{
			delete mpImpl;
			mpImpl = other.mpImpl;
			other.mpImpl = nullptr;
		}

		return *this;
	}

	Vector::~Vector() noexcept
	{
		delete mpImpl;
	}

	Vector::Vector(Vec4Impl&& other) noexcept
		: mpImpl(new Vec4Impl(std::move(other)))
	{}

	bool Vector::operator==(const Vector& other) const
	{
		return *mpImpl == *other.mpImpl;
	}

	bool Vector::operator!=(const Vector& other) const
	{
		return !(*this == other);
	}

	float& Vector::operator[](int index)
	{
		return (*mpImpl)[index];
	}

	float Vector::operator[](int index) const
	{
		return (*mpImpl)[index];
	}

	float Vector::operator*(const Vector& other) const
	{
		return (*mpImpl) * (*other.mpImpl);
	}

	Vector Vector::operator+(const Vector& other) const
	{
		return (*mpImpl) + (*other.mpImpl);
	}

	void Vector::operator+=(const Vector& other)
	{
		(*mpImpl) += (*other.mpImpl);
	}

	Vector Vector::operator-(const Vector& other) const
	{
		return (*mpImpl) - (*other.mpImpl);
	}

	Vector Vector::operator/(float n) const
	{
		return (*mpImpl) / n;
	}

	Vector Vector::operator*(float n) const
	{
		return (*mpImpl) * n;
	}

	Vector operator*(float n, const Vector& other)
	{
		return other * n;
	}

	const float* Vector::valuePtr() const
	{
		return mpImpl->valuePtr();
	}

	Vector Vector::getNormalized() const
	{
		return mpImpl->getNormalized();
	}

	Vector Vector::operator-() const
	{
		return -(*mpImpl);
	}

	float Vector::length() const
	{
		return mpImpl->length();
	}

	const char* Vector::getPrettyString() const
	{
		return mpImpl->getPrettyString();
	}

	float Vector::x() const
	{
		return (*mpImpl)[0];
	}

	void Vector::setX(float x)
	{
		(*mpImpl)[0] = x;
	}

	float Vector::y() const
	{
		return (*mpImpl)[1];
	}

	void Vector::setY(float y)
	{
		(*mpImpl)[1] = y;
	}

	float Vector::r() const
	{
		return x();
	}

	void Vector::setR(float r)
	{
		setX(r);
	}

	float Vector::g() const
	{
		return y();
	}

	void Vector::setG(float g)
	{
		setY(g);
	}

	float Vector::z() const
	{
		return (*mpImpl)[2];
	}

	void Vector::setZ(float z)
	{
		(*mpImpl)[2] = z;
	}

	float Vector::b() const
	{
		return z();
	}

	void Vector::setB(float b)
	{
		setZ(b);
	}

	float Vector::w() const
	{
		return (*mpImpl)[3];
	}

	void Vector::setW(float w)
	{
		(*mpImpl)[3] = w;
	}

	float Vector::a() const
	{
		return w();
	}

	void Vector::setA(float a)
	{
		setW(a);
	}

	Vec4Impl* Vector::__internal_getPimpl() const
	{
		return mpImpl;
	}

	Vector2D::Vector2D(float x, float y)
		: Vector(x, y, 0.0f, 0.0f)
	{}

	Vector2D::Vector2D(const Vector3D& vec3)
		: Vector(vec3.x(), vec3.y(), 0.0f, 0.0f)
	{}

	Vector2D::Vector2D(Vector3D&& vec3)
		: Vector(std::move(vec3))
	{
		Vector::setZ(0.0f);
	}

	Vector2D::Vector2D(Vec4Impl&& other) noexcept
		: Vector(std::move(other))
	{}

	Vector2D::Vector2D(const Vector& other) noexcept
		: Vector(other)
	{}

	Vector2D::Vector2D(Vector&& other) noexcept
		: Vector(std::move(other))
	{}

	Vector3D::Vector3D(float x, float y, float z)
		: Vector(x, y, z, 0.0f)
	{}

	Vector3D::Vector3D(const Vector2D& vec2, float z) noexcept
		: Vector(vec2.x(), vec2.y(), z, 0.0f)
	{}

	Vector3D::Vector3D(const Vector4D& vec4) noexcept
		: Vector(vec4.x(), vec4.y(), vec4.z(), 0.0f)
	{}

	Vector3D::Vector3D(Vector4D&& vec4) noexcept
		: Vector(std::move(vec4))
	{
		Vector::setW(0.0f);
	}

	Vector3D::Vector3D(Vec4Impl&& other) noexcept
		: Vector(std::move(other))
	{}

	Vector3D::Vector3D(const Vector& other) noexcept
		: Vector(other)
	{}

	Vector3D::Vector3D(Vector&& other) noexcept
		: Vector(std::move(other))
	{}

	Vector3D Vector3D::cross(const Vector3D& other) const
	{
		return mpImpl->cross(*other.mpImpl);
	}

	Vector3D Vector3D::project(const Matrix4D& modelView, const Matrix4D& proj, const Vector4D& viewport) const
	{
		return mpImpl->project(modelView, proj, *viewport.__internal_getPimpl());
	}

	Vector3D Vector3D::unProject(const Matrix4D& modelView, const Matrix4D& proj, const Vector4D& viewport) const
	{
		return mpImpl->unProject(modelView, proj, *viewport.__internal_getPimpl());
	}

	Vector4D::Vector4D(float x, float y, float z, float w)
		: Vector(x, y, z, w)
	{}

	Vector4D::Vector4D(const Vector2D& vec2, float z, float w)
		: Vector(vec2.x(), vec2.y(), z, w)
	{}

	Vector4D::Vector4D(const Vector3D& vec3, float w)
		: Vector(vec3.x(), vec3.y(), vec3.z(), w)
	{}

	Vector4D::Vector4D(Vec4Impl&& other) noexcept
		: Vector(std::move(other))
	{}

	Vector4D::Vector4D(const Vector& other) noexcept
		: Vector(other)
	{}

	Vector4D::Vector4D(Vector&& other) noexcept
		: Vector(std::move(other))
	{}

	Vector3D Vector4D::getVec3() const
	{
		return Vector3D(x(), y(), z());
	}
}
