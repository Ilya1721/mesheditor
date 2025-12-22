#pragma once

#include <string>
#include <unordered_set>

namespace MeshFilesLoader
{
  constexpr int STL_HEADER_SIZE = 80;
  const inline std::string DELIMITERS = " ,\t\n/";
  const std::unordered_set<std::string> SUPPORTED_TEXTURE_EXTENSIONS = {
    ".png", ".jpg", ".jpeg", ".bmp", ".tga"
  };
}  // namespace MeshFilesLoader
