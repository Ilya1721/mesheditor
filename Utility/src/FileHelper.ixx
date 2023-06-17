export module FileHelper;

export import <filesystem>;

export namespace Helpers
{
	std::string readFile(const std::filesystem::path& filePath);
}

