#include "pch.h"

#include "EqualityUtils.h"

#include <glm/gtc/epsilon.hpp>

#include "Constants.h"

namespace Utility
{
	bool EqualityUtils::stringEquals(const std::string& firstString, const std::string& secondString, bool caseSensitive) noexcept
	{
		if (firstString.size() != secondString.size() || firstString.empty() || secondString.empty())
		{
			return false;
		}

		for (size_t strIdx = 0; strIdx < firstString.size(); ++strIdx)
		{
			auto firstSymbol = caseSensitive ? std::tolower(firstString[strIdx]) : firstString[strIdx];
			auto secondSymbol = caseSensitive ? std::tolower(secondString[strIdx]) : secondString[strIdx];

			if (firstSymbol != secondSymbol)
			{
				return false;
			}
		}

		return true;
	}

	bool EqualityUtils::isEqual(const glm::vec3& firstVec, const glm::vec3& secondVec) noexcept
	{
		return glm::all(glm::epsilonEqual(firstVec, secondVec, Constants::EPSILON));
	}
}
