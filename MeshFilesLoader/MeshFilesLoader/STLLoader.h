#pragma once

#include "IFileLoader.h"

namespace MeshFilesLoader
{
	class STLLoader : public IFileLoader
	{
	public:
		API MeshCore::Mesh loadMesh(const std::filesystem::path& filePath) const noexcept override;
		API void saveMesh(const std::filesystem::path& filePath, const MeshCore::Mesh& meshToSave) const noexcept override {};
	};
}

