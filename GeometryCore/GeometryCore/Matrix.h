#pragma once

#include "SolutionMacros.h"

namespace Geometry
{
	template<int Rows, int Cols>
	class MatImpl;

	template<int Rows, int Cols>
	class API Matrix final
	{
	public:
		Matrix() noexcept;
		Matrix(double arg) noexcept;

		Matrix(const Matrix& other) noexcept;
		Matrix(Matrix&& other) noexcept;
		Matrix& operator=(const Matrix& other) noexcept;
		Matrix& operator=(Matrix&& other) noexcept;
		~Matrix() noexcept;

		bool operator==(const Matrix& other) const noexcept;

	private:
		MatImpl<Rows, Cols>* mpImpl;
	};

	using Matrix2D = Matrix<2, 2>;
	using Matrix3D = Matrix<3, 3>;
	using Matrix4D = Matrix<4, 4>;
}

