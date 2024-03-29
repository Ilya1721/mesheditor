#include "FileHelper.h"

#include <fstream>

namespace Utility
{
	std::string readFile(const std::filesystem::path& filePath)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (!file.is_open())
		{
			throw std::exception("Could not open the file");
		}

		file.seekg(0, std::ios::end);
		size_t fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		std::string content(fileSize, '\0');
		file.read(content.data(), fileSize);
		if (file.bad())
		{
			throw std::exception("Could not read the file");
		}

		return content;
	}
}
