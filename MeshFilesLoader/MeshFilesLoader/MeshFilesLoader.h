#pragma once

#include <filesystem>

#include <MeshCore/Mesh.h>

namespace MeshFilesLoader
{
	std::unique_ptr<MeshCore::Mesh> loadSTL(const std::filesystem::path& filePath);
}