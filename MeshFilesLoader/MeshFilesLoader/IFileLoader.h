#pragma once

#include <filesystem>

#include "MeshCore/Mesh.h"

namespace MeshFilesLoader
{
	class IFileLoader
	{
	public:
		API virtual MeshCore::Mesh loadMesh(const std::filesystem::path& filePath) const noexcept = 0;
		API virtual void saveMesh(const std::filesystem::path& filePath, const MeshCore::Mesh& meshToSave) const noexcept = 0;
	};
}
