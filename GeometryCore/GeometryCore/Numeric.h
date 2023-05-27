#pragma once

#include "SolutionMacros.h"

#include <type_traits>

namespace Geometry
{
	template <typename Numeric, std::enable_if_t<std::is_arithmetic_v<Numeric>, bool>>
	API bool isEqual(Numeric first, Numeric second) noexcept
	{
		return std::abs(first - second) < Geometry::EPSILON;
	}
}
