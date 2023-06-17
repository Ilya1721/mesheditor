export module StringHelper;

export import <iostream>;

export namespace Helpers
{
	bool isEqual(const std::string& firstString, const std::string& secondString, bool caseSensitive = false) noexcept;
}

