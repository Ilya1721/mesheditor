#include "TestUtils.h"

#include <filesystem>

#include "MeshFilesLoader/MeshFilesLoader.h"

using namespace std::filesystem;

MeshCore::Mesh getDefaultMesh()
{
	std::vector<MeshCore::Vertex> vertices;
	vertices.push_back(defaultVertex);

	return { vertices };
}

MeshCore::Object3D getDefaultObject3D()
{
	return { nullptr, getDefaultMesh() };
}

MeshCore::Mesh loadCube()
{
	return MeshFilesLoader::loadSTL(current_path() / path(R"(..\..\Tests\TestFiles\CubeText.stl)"));
}