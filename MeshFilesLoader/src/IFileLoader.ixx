export module IFileLoader;

import <filesystem>;

import Mesh;

export namespace MeshFilesLoader
{
	class IFileLoader
	{
	public:
		virtual MeshCore::Mesh loadMesh(const std::filesystem::path& filePath) const noexcept = 0;
		virtual void saveMesh(const std::filesystem::path& filePath, const MeshCore::Mesh& meshToSave) const noexcept = 0;
	};
}
