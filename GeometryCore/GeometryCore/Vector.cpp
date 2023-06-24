#include "Vector.h"

#include "VecImpl.h"

namespace Geometry
{
	Vector2D::Vector2D() noexcept
		: mpImpl(new Vec4Impl())
	{}

	Vector2D::Vector2D(float x, float y) noexcept
		: mpImpl(new Vec4Impl(x, y, 0.0f, 0.0f))
	{}

	Vector2D::Vector2D(const Vector2D& other) noexcept
		: mpImpl(new Vec4Impl(*other.mpImpl))
	{}

	Vector2D::Vector2D(Vector2D&& other) noexcept
	{
		*this = std::move(other);
	}

	Vector2D& Vector2D::operator=(const Vector2D& other) noexcept
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

	Vector2D::~Vector2D() noexcept
	{
		delete mpImpl;
	}

	Vector2D::Vector2D(Vec4Impl&& other) noexcept
		: mpImpl(new Vec4Impl(other))
	{}

	bool Vector2D::operator==(const Vector2D& other) const noexcept
	{
		return *mpImpl == *other.mpImpl;
	}

	float& Vector2D::operator[](int index) noexcept
	{
		return (*mpImpl)[index];
	}

	float Vector2D::operator[](int index) const noexcept
	{
		return (*mpImpl)[index];
	}

	Vector2D Vector2D::operator+(const Vector2D& other) const noexcept
	{
		return (*mpImpl) + (*other.mpImpl);
	}

	void Vector2D::operator+=(const Vector2D& other) noexcept
	{
		(*mpImpl) += (*other.mpImpl);
	}

	Vector2D Vector2D::operator-(const Vector2D& other) const noexcept
	{
		return (*mpImpl) - (*other.mpImpl);
	}

	Vector2D Vector2D::operator/(float n) const noexcept
	{
		return (*mpImpl) / n;
	}

	float Vector2D::x() const noexcept
	{
		return (*mpImpl)[0];
	}

	void Vector2D::setX(float x) noexcept
	{
		(*mpImpl)[0] = x;
	}

	float Vector2D::y() const noexcept
	{
		return (*mpImpl)[1];
	}

	void Vector2D::setY(float y) noexcept
	{
		(*mpImpl)[1] = y;
	}

	float Vector2D::r() const noexcept
	{
		return x();
	}

	void Vector2D::setR(float r) noexcept
	{
		setX(r);
	}

	float Vector2D::g() const noexcept
	{
		return y();
	}

	void Vector2D::setG(float g) noexcept
	{
		setY(g);
	}

	Vec4Impl* Vector2D::__internal_getPimpl() const noexcept
	{
		return mpImpl;
	}

	Vector2D Vector2D::normalize(const Vector2D& vec) noexcept
	{
		return Vec4Impl::normalize(*vec.mpImpl);
	}

	float Vector2D::dot(const Vector2D& firstVec, const Vector2D& secondVec) noexcept
	{
		return Vec4Impl::dot(*firstVec.mpImpl, *secondVec.mpImpl);
	}

	Vector2D Vector2D::cross(const Vector2D& firstVec, const Vector2D& secondVec) noexcept
	{
		return Vec4Impl::cross(*firstVec.mpImpl, *secondVec.mpImpl);
	}

	Vector3D::Vector3D(float x, float y, float z) noexcept
		: Vector2D(x, y)
	{
		setZ(z);
	}

	Vector3D::Vector3D(Vector2D&& other) noexcept
		: Vector2D(other)
	{}

	Vector3D::Vector3D(const Vector3D& other) noexcept
		: Vector2D(other)
	{}

	Vector3D::Vector3D(Vector3D&& other) noexcept
		: Vector2D(other)
	{}

	Vector3D::Vector3D(Vec4Impl&& other) noexcept
		: Vector2D(std::move(other))
	{}

	Vector3D& Vector3D::operator=(const Vector3D& other) noexcept
	{
		Vector2D::operator=(other);
		return *this;
	}

	Vector3D& Vector3D::operator=(Vector3D&& other) noexcept
	{
		Vector2D::operator=(other);
		return *this;
	}

	Vector3D Vector3D::operator+(const Vector3D& other) const noexcept
	{
		return Vector2D::operator+(other);
	}

	Vector3D Vector3D::operator-(const Vector3D& other) const noexcept
	{
		return Vector2D::operator-(other);
	}

	Vector3D Vector3D::operator/(float n) const noexcept
	{
		return Vector2D::operator/(n);
	}

	float Vector3D::z() const noexcept
	{
		return (*mpImpl)[2];
	}

	void Vector3D::setZ(float z) noexcept
	{
		(*mpImpl)[2] = z;
	}

	float Vector3D::b() const noexcept
	{
		return z();
	}

	void Vector3D::setB(float b) noexcept
	{
		setZ(b);
	}

	Vector3D Vector3D::normalize(const Vector3D& vec) noexcept
	{
		return Vector2D::normalize(vec);
	}

	float Vector3D::dot(const Vector3D& firstVec, const Vector3D& secondVec) noexcept
	{
		return Vector2D::dot(firstVec, secondVec);
	}

	Vector3D Vector3D::cross(const Vector3D& firstVec, const Vector3D& secondVec) noexcept
	{
		return Vector2D::cross(firstVec, secondVec);
	}

	Vector3D Vector3D::project(const Vector3D& obj, const Matrix4D& model, const Matrix4D& proj, const Vector4D& viewport) noexcept
	{
		return Vec4Impl::project(*obj.mpImpl, model, proj, *viewport.mpImpl);
	}

	Vector3D Vector3D::unProject(const Vector3D& win, const Matrix4D& model, const Matrix4D& proj, const Vector4D& viewport) noexcept
	{
		return Vec4Impl::unProject(*win.mpImpl, model, proj, *viewport.mpImpl);
	}

	Vector4D::Vector4D(float x, float y, float z, float w) noexcept
		: Vector3D(x, y, z)
	{
		setW(w);
	}

	Vector4D::Vector4D(Vector3D&& other) noexcept
		: Vector3D(other)
	{}

	Vector4D::Vector4D(Vec4Impl&& other) noexcept
		: Vector3D(std::move(other))
	{}

	Vector4D::Vector4D(const Vector4D& other) noexcept
		: Vector3D(other)
	{}

	Vector4D::Vector4D(Vector4D&& other) noexcept
		: Vector3D(other)
	{}

	Vector4D& Vector4D::operator=(const Vector4D& other) noexcept
	{
		Vector3D::operator=(other);
		return *this;
	}

	Vector4D& Vector4D::operator=(Vector4D&& other) noexcept
	{
		Vector3D::operator=(other);
		return *this;
	}

	Vector4D Vector4D::operator+(const Vector4D& other) const noexcept
	{
		return Vector3D::operator+(other);
	}

	Vector4D Vector4D::operator-(const Vector4D& other) const noexcept
	{
		return Vector3D::operator-(other);
	}

	Vector4D Vector4D::operator/(float n) const noexcept
	{
		return Vector3D::operator/(n);
	}

	Vector4D Vector4D::operator*(const Matrix4D& matrix) const noexcept
	{
		return (*mpImpl) * matrix;
	}

	Vector3D Vector4D::getVec3() const noexcept
	{
		return Vector3D(x(), y(), z());
	}

	float Vector4D::w() const noexcept
	{
		return (*mpImpl)[3];
	}

	void Vector4D::setW(float w) noexcept
	{
		(*mpImpl)[3] = w;
	}

	float Vector4D::a() const noexcept
	{
		return w();
	}

	void Vector4D::setA(float a) noexcept
	{
		setW(a);
	}

	Vector4D Vector4D::normalize(const Vector4D& vec) noexcept
	{
		return Vector3D::normalize(vec);
	}

	float Vector4D::dot(const Vector4D& firstVec, const Vector4D& secondVec) noexcept
	{
		return Vector3D::dot(firstVec, secondVec);
	}

	Vector4D Vector4D::cross(const Vector4D& firstVec, const Vector4D& secondVec) noexcept
	{
		return Vector3D::cross(firstVec, secondVec);
	}
}
