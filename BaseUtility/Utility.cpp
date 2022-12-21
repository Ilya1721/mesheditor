#include "pch.h"

#include "Utility.h"

#include <glm/gtc/epsilon.hpp>

#include "Constants.h"

namespace BaseUtility
{
	std::string Utility::readFile(const std::filesystem::path& filePath)
	{
		if (filePath.empty())
		{
			throw std::exception("File path is empty");
		}

		std::ifstream file(filePath);
		if (!file.is_open())
		{
			throw std::exception("File was not opened");
		}

		file.seekg(0, std::ios::end);
		size_t fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		std::string content(fileSize, '\0');
		file.read(content.data(), fileSize);

		if (file.bad())
		{
			throw std::exception("file was not read");
		}

		return content;
	}

	bool Utility::stringEquals(const std::string& firstString, const std::string& secondString, bool caseSensitive) noexcept
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

	bool Utility::isEqual(const glm::vec3& firstVec, const glm::vec3& secondVec) noexcept
	{
		return glm::all(glm::epsilonEqual(firstVec, secondVec, Constants::EPSILON));
	}
}
