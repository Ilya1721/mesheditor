#pragma once

#include <string>
#include <filesystem>

#include "SolutionMacros.h"

namespace Utility
{
	class FileUtils
	{
	public:
		API static std::string readFile(const std::filesystem::path& filePath);
	};
}

