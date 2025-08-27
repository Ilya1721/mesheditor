#pragma once

#include <filesystem>

namespace MeshCore
{
  class Mesh;
}

namespace MeshFilesLoader
{
  std::unique_ptr<MeshCore::Mesh> loadMeshFromFile(const std::filesystem::path& filePath);
}