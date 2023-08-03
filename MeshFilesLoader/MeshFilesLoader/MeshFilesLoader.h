#pragma once

#include <filesystem>

#include <MeshCore/Mesh.h>

namespace MeshFilesLoader
{
	MeshCore::Mesh loadSTL(const std::filesystem::path& filePath);
}