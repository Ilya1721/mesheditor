#pragma once

#include <string>
#include <filesystem>

#include <glm/glm.hpp>

#include "Constants.h"

namespace BaseUtility
{
	class Utility final
	{
	public:
		Utility() = delete;

		__declspec(dllexport) static std::string readFile(const std::filesystem::path& filePath);

		__declspec(dllexport) static bool stringEquals(const std::string& firstString, const std::string& secondString, bool caseSensitive = false) noexcept;

		__declspec(dllexport) static bool isEqual(const glm::vec3& firstVec, const glm::vec3& secondVec) noexcept;

		template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
		static bool isEqual(T firstNum, T secondNum) noexcept
		{
			return std::abs(firstNum - secondNum) < Constants::EPSILON;
		}
	};
}

