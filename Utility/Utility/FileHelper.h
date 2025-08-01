#pragma once

#include <filesystem>

namespace Utility
{
  std::string readFile(const std::filesystem::path& filePath);
}
