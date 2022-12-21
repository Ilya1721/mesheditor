#pragma once

#include "IFileLoader.h"

namespace MeshFilesLoader
{
	class STLLoader : public IFileLoader
	{
	public:
		__declspec(dllexport) MeshCore::Mesh loadMesh(const std::filesystem::path& filePath) const noexcept override;
		__declspec(dllexport) void saveMesh(const std::filesystem::path& filePath, const MeshCore::Mesh& meshToSave) const noexcept override {};
	};
}

