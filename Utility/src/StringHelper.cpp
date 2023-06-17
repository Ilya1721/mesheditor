module StringHelper;

namespace Helpers
{
	bool isEqual(const std::string& firstString, const std::string& secondString, bool caseSensitive) noexcept
	{
		if (firstString.size() != secondString.size())
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
}
