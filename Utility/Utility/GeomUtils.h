#pragma once

#include <type_traits>

#include "SolutionMacros.h"

namespace Utility
{
	class GeomUtils
	{
	public:
		template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
		API static T convertToRange(T number, T oldMin, T oldMax, T newMin, T newMax) noexcept
		{
			return number * (newMax - newMin) / (oldMax - oldMin) + newMin;
		}
	};
}

