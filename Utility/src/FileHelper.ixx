export module FileHelper;

#include "SolutionMacros.h"

import std;

export namespace Helpers
{
	API std::string readFile(const std::filesystem::path& filePath);
}

