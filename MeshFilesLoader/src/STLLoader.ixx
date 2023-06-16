export module STLLoader;

import IFileLoader;

export namespace MeshFilesLoader
{
	class STLLoader : public IFileLoader
	{
	public:
		MeshCore::Mesh loadMesh(const std::filesystem::path& filePath) const noexcept override;
		void saveMesh(const std::filesystem::path& filePath, const MeshCore::Mesh& meshToSave) const noexcept override {};
	};
}

