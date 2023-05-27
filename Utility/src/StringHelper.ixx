export module StringHelper;

#include "SolutionMacros.h"

import std;

export namespace Helpers
{
	API bool isEqual(const std::string& firstString, const std::string& secondString, bool caseSensitive = false) noexcept;
}

