#pragma once

#include <filesystem>

#include "MeshCore/Mesh.h"

namespace MeshFilesLoader
{
	class IFileLoader
	{
	public:
		__declspec(dllexport) virtual MeshCore::Mesh loadMesh(const std::filesystem::path& filePath) const noexcept = 0;
		__declspec(dllexport) virtual void saveMesh(const std::filesystem::path& filePath, const MeshCore::Mesh& meshToSave) const noexcept = 0;
	};
}
