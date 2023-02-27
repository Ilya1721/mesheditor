#pragma once

#include <string>
#include <filesystem>

#include <glm/glm.hpp>

#include "Constants.h"
#include "SolutionMacros.h"

namespace Utility
{
	class EqualityUtils final
	{
	public:
		EqualityUtils() = delete;

		API static bool stringEquals(const std::string& firstString, const std::string& secondString, bool caseSensitive = false) noexcept;

		API static bool isEqual(const glm::vec3& firstVec, const glm::vec3& secondVec) noexcept;

		template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
		static bool isEqual(T firstNum, T secondNum) noexcept
		{
			return std::abs(firstNum - secondNum) < Constants::EPSILON;
		}
	};
}

