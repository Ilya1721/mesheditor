#include "Matrix.h"

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "Constants.h"

namespace Geometry
{
	template<int Rows, int Cols>
	using MatType = glm::mat<Rows, Cols, double, glm::qualifier::defaultp>;

	template<int Rows, int Cols>
	class MatImpl
	{
	public:
		MatImpl() noexcept;
		MatImpl(double arg) noexcept;

		MatImpl(const MatImpl& other) noexcept;
		MatImpl(MatImpl&& other) noexcept;
		MatImpl& operator=(const MatImpl& other) noexcept;
		MatImpl& operator=(MatImpl&& other) noexcept;

		bool operator==(const MatImpl& other) const noexcept;

	private:
		std::unique_ptr<MatType<Rows, Cols>> mpImplMat;
	};

	template<int Rows, int Cols>
	MatImpl<Rows, Cols>::MatImpl() noexcept
		: mpImplMat(std::make_unique<MatType>())
	{}

	template<int Rows, int Cols>
	MatImpl<Rows, Cols>::MatImpl(double arg) noexcept
		: mpImplMat(std::make_unique<MatType>(arg))
	{}

	template<int Rows, int Cols>
	MatImpl<Rows, Cols>::MatImpl(const MatImpl& other) noexcept
		: mpImplMat(std::make_unique(*other.mpImplMat))
	{}

	template<int Rows, int Cols>
	MatImpl<Rows, Cols>::MatImpl(MatImpl&& other) noexcept
	{
		*this = std::move(other);
	}

	template<int Rows, int Cols>
	MatImpl<Rows, Cols>& MatImpl<Rows, Cols>::operator=(MatImpl&& other) noexcept
	{
		if (this != &other)
		{
			mpImplMat = std::move(other.mpImplMat);
		}

		return *this;
	}

	template<int Rows, int Cols>
	MatImpl<Rows, Cols>& MatImpl<Rows, Cols>::operator=(const MatImpl& other) noexcept
	{
		if (this != &other)
		{
			*mpImplMat = *other.mpImplMat;
		}

		return *this;
	}

	template<int Rows, int Cols>
	bool MatImpl<Rows, Cols>::operator==(const MatImpl& other) const noexcept
	{
		return glm::all(glm::epsilonEqual(*mpImplMat, *other.mpImplMat, EPSILON));
	}
}

namespace Geometry
{
	template<int Rows, int Cols>
	Matrix<Rows, Cols>::Matrix() noexcept
		: mpImpl(new MatImpl<Rows, Cols>())
	{}

	template<int Rows, int Cols>
	Matrix<Rows, Cols>::Matrix(double arg) noexcept
		: mpImpl(new MatImpl<Rows, Cols>(arg))
	{}

	template<int Rows, int Cols>
	Matrix<Rows, Cols>::Matrix(const Matrix& other) noexcept
		: mpImpl(new MatImpl<Rows, Cols>(*other.mpImpl))
	{}

	template<int Rows, int Cols>
	Matrix<Rows, Cols>::Matrix(Matrix&& other) noexcept
	{
		*this = std::move(other);
	}

	template<int Rows, int Cols>
	Matrix<Rows, Cols>& Matrix<Rows, Cols>::operator=(const Matrix& other) noexcept
	{
		if (this != &other)
		{
			*mpImpl = *other.mpImpl;
		}

		return *this;
	}

	template<int Rows, int Cols>
	Matrix<Rows, Cols>& Matrix<Rows, Cols>::operator=(Matrix&& othertor) noexcept
	{
		if (this != &othertor)
		{
			delete mpImpl;
			mpImpl = othertor.mpImpl;
			othertor.mpImpl = nullptr;
		}

		return *this;
	}

	template<int Rows, int Cols>
	Matrix<Rows, Cols>::~Matrix() noexcept
	{
		delete mpImpl;
	}

	template<int Rows, int Cols>
	bool Matrix<Rows, Cols>::operator==(const Matrix& other) const noexcept
	{
		return *mpImpl == *other.mpImpl;
	}
}
