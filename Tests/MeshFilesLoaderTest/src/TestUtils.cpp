#include "TestUtils.h"

#include <filesystem>

#include "MeshFilesLoader/MeshFilesLoader.h"

using namespace std::filesystem;

MeshCore::Mesh loadCube()
{
	return MeshFilesLoader::loadSTL(current_path() / path(R"(..\..\Tests\TestFiles\CubeText.stl)"));
}
