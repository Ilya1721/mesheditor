#include "StringHelper.h"

namespace Utility
{
	bool isEqual(const std::string& firstString, const std::string& secondString, bool caseSensitive)
	{
		return std::equal(firstString.begin(), firstString.end(), secondString.begin(), secondString.end(),
		[caseSensitive](char first, char second) {
			return caseSensitive ? first == second : std::tolower(first) == std::tolower(second);
		});
	}
}
