#include "Vector.h"

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "Constants.h"

namespace Geometry
{
	template<int Dimension>
	using VecType = glm::vec<Dimension, double, glm::qualifier::defaultp>;

	template<int Dimension>
	class VecImpl
	{
	public:
		VecImpl() noexcept;
		VecImpl(double arg) noexcept;
		VecImpl(double args...) noexcept;

		VecImpl(const VecImpl& other) noexcept;
		VecImpl(VecImpl&& other) noexcept;
		VecImpl& operator=(const VecImpl& other) noexcept;
		VecImpl& operator=(VecImpl&& other) noexcept;

		bool operator==(const VecImpl& other) const noexcept;

	private:
		std::unique_ptr<VecType<Dimension>> mpImplVec;
	};

	template<int Dimension>
	VecImpl<Dimension>::VecImpl() noexcept
		: mpImplVec(std::make_unique<VecType>())
	{}

	template<int Dimension>
	VecImpl<Dimension>::VecImpl(double arg) noexcept
		: mpImplVec(std::make_unique<VecType>(arg))
	{}

	template<int Dimension>
	VecImpl<Dimension>::VecImpl(double args...) noexcept
		: mpImplVec(std::make_unique<VecType>(args...))
	{}

	template<int Dimension>
	VecImpl<Dimension>::VecImpl(const VecImpl& other) noexcept
		: mpImplVec(std::make_unique(*other.mpImplVec))
	{}

	template<int Dimension>
	VecImpl<Dimension>::VecImpl(VecImpl&& other) noexcept
	{
		*this = std::move(other);
	}

	template<int Dimension>
	VecImpl<Dimension>& VecImpl<Dimension>::operator=(VecImpl&& other) noexcept
	{
		if (this != &other)
		{
			mpImplVec = std::move(other.mpImplVec);
		}

		return *this;
	}

	template<int Dimension>
	VecImpl<Dimension>& VecImpl<Dimension>::operator=(const VecImpl& other) noexcept
	{
		if (this != &other)
		{
			*mpImplVec = *other.mpImplVec;
		}

		return *this;
	}

	template<int Dimension>
	bool VecImpl<Dimension>::operator==(const VecImpl& other) const noexcept
	{
		return glm::all(glm::epsilonEqual(*mpImplVec, *other.mpImplVec, EPSILON));
	}
}

namespace Geometry
{
	template<int Dimension>
	Vector<Dimension>::Vector() noexcept
		: mpImpl(new VecImpl<Dimension>())
	{}

	template<int Dimension>
	Vector<Dimension>::Vector(double arg) noexcept
		: mpImpl(new VecImpl<Dimension>(arg))
	{}

	template<int Dimension>
	Vector<Dimension>::Vector(double args...) noexcept
		: mpImpl(new VecImpl<Dimension>(args...))
	{}

	template<int Dimension>
	Vector<Dimension>::Vector(const Vector& other) noexcept
		: mpImpl(new VecImpl<Dimension>(*other.mpImpl))
	{}

	template<int Dimension>
	Vector<Dimension>::Vector(Vector&& other) noexcept
	{
		*this = std::move(other);
	}

	template<int Dimension>
	Vector<Dimension>& Vector<Dimension>::operator=(const Vector& other) noexcept
	{
		if (this != &other)
		{
			*mpImpl = *other.mpImpl;
		}

		return *this;
	}

	template<int Dimension>
	Vector<Dimension>& Vector<Dimension>::operator=(Vector&& othertor) noexcept
	{
		if (this != &othertor)
		{
			delete mpImpl;
			mpImpl = othertor.mpImpl;
			othertor.mpImpl = nullptr;
		}

		return *this;
	}

	template<int Dimension>
	Vector<Dimension>::~Vector() noexcept
	{
		delete mpImpl;
	}

	template<int Dimension>
	bool Vector<Dimension>::operator==(const Vector& other) const noexcept
	{
		return *mpImpl == *other.mpImpl;
	}
}
