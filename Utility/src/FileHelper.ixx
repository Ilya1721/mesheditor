export module FileHelper;

import std;

export namespace Helpers
{
	std::string readFile(const std::filesystem::path& filePath);
}

