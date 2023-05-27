#pragma once

#include "SolutionMacros.h"

namespace Geometry
{
	template<int Dimension>
	class VecImpl;

	template<int Dimension>
	class API Vector final
	{
	public:
		Vector() noexcept;
		Vector(double arg) noexcept;
		Vector(double args...) noexcept;

		Vector(const Vector& other) noexcept;
		Vector(Vector&& other) noexcept;
		Vector& operator=(const Vector& other) noexcept;
		Vector& operator=(Vector&& other) noexcept;
		~Vector() noexcept;

		bool operator==(const Vector& other) const noexcept;

	private:
		VecImpl<Dimension>* mpImpl;
	};

	using Vector2D = Vector<2>;
	using Vector3D = Vector<3>;
	using Vector4D = Vector<4>;
}

