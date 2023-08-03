#pragma once

#include <filesystem>

namespace Helpers
{
	std::string readFile(const std::filesystem::path& filePath);
}

